// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <opencv/cv.h>

#include "AlgorithmBase.h"
#include "PeakVector.h"
#include "PeakFinder.h"
#include "WhiteoutPerimeter.h"
#include "ConnFinder.h"
#include "PortMapper.h"
#include "AlgorithmBase.h"

namespace cs
{

// -----------------------------------------------------------------------------
// Static configuration
// -----------------------------------------------------------------------------

// The max dimension of the resized blob image that we will apply filters to.
static unsigned int g_smallres = 500;

// The peak scale value for tesseract (common for circuits and english).
static double g_peakScaleTesseract = 2.0;

// Additionally scale English values by this much.
static double g_peakScaleEnglish = 2.0;

// Peak scale value for merging to find peaks. Currently do not merge.
static unsigned int g_peakScaleMerge = 1.0;

// The cutoff confidence value for circuit components.
static unsigned int g_cutoffCircuits = 75;

// The cutoff confidence value for text entities.
static unsigned int g_cutoffEnglish = 75;

// -----------------------------------------------------------------------------
// The algorithm.
// -----------------------------------------------------------------------------

void AlgorithmBase::ConvertToNetlist(unsigned int thresholdStart,
        unsigned int thresholdEnd)
{
    // 3-channel blob image.
    IplImage* blob = 0;
    Filter filter(m_src, g_smallres);

    CHECKPOINT_STARTING(filter);

    // Save the best text circuit for later iterations.
    Circuit bestTextCircuit;
    PeakVector bestPeaks(Space(0,0));

    // Must run at least once.
    if (thresholdEnd <= thresholdStart)
    {
        thresholdEnd = thresholdStart+1;
    }

    // For each threshold to evaluate...
    for (unsigned int threshold = thresholdStart; threshold < thresholdEnd;
            threshold++)
    {
        CHECKPOINT_NEWTHRESH(threshold);

        // Find the peaks
        const IplImage* compblob = filter.ComponentBlob(threshold);
        if (blob == 0)
        {
            // Only initialize the blob once.
            blob = cvCreateImage(cvGetSize(compblob), IPL_DEPTH_8U, 3);
        }

        cvCvtColor(compblob, blob, CV_GRAY2BGR);
        WhiteoutPerimeter whiteout1(*blob);
        whiteout1.DoWhiteoutPerimeter();
        PeakFinder peakfinder(*blob);

        CHECKPOINT_FOUNDPEAKS(*blob);
        PeakVector& peaks = peakfinder.FindPeaks(g_peakScaleMerge);

        // Filter the image and get the connection blob
        const IplImage* connblob = filter.ConnectionBlob();
        cvCvtColor(connblob, blob, CV_GRAY2BGR);

        // Whiteout the connblob perimeter.
        WhiteoutPerimeter whiteout2(*blob);
        whiteout2.DoWhiteoutPerimeter();

        // Scale the peaks
        peaks.Scale(g_peakScaleTesseract);

        // Find and print the connections.
        ConnFinder connfinder(*blob, peaks);
        const ConnVector& conns = connfinder.GetConnections();

        // Construct the circuit.
        PortMapper mapper(peaks, conns);
        Circuit circuit = mapper.ConvertToCircuit();

        // Apply tesseract.
        Transform transform(
                Space(blob->width, blob->height),
                Space(m_src.width, m_src.height));
        Tesseract tess(m_src);

        // The confidence value for this circuit.
        unsigned int confidence = 0;
        unsigned int textConfidence = 0;

        CHECKPOINT_FOUNDCONNS(*blob);

        unsigned int averageArea = peaks.GetAverageArea();
        for (unsigned int i = 0; i < circuit.GetNumEntities(); i++)
        {
            Rectangle map = transform.Map(peaks.GetVector()[i]);
            CHECKPOINT_TESSERACT(map);

            // Don't apply tesseract for huge components. They are definitely
            // wrong.
            if (map.GetArea()*2 > m_src.width*m_src.height)
            {
                continue;
            }

            if (circuit.IsConnected(i))
            {
                // Ignore the peak if it is too small. Circuits only.
                if (peaks.GetVector()[i].GetArea()*2 < averageArea)
                {
                    continue;
                }

                // Circuit component.
                ResultCircuits cresult = tess.TesseractCircuits(map);

                if (cresult.GetConfidence() > g_cutoffCircuits &&
                    !cresult.GetVector().empty())
                {
                    // Only take the first one, even if there are multiple
                    // components.
                    SmartComponentID id(cresult.GetVector()[0].GetID());
                    circuit.SetComponentID(i, id);

                    confidence += cresult.GetConfidence();
                }

                CHECKPOINT_CIRCUITS(cresult);
            }
            else
            {
                // Scale the map rect some more, for english only.
                Rectangle emap = map;
                emap.Scale(g_peakScaleEnglish);

                // Text component.
                ResultEnglish eresult = tess.TesseractEnglish(emap);

                if (eresult.GetConfidence() > g_cutoffEnglish)
                {
                    // Set this value to be used later.
                    circuit.SetValue(i, eresult.GetChars());
                    textConfidence += eresult.GetConfidence();
                }

                CHECKPOINT_ENGLISH(eresult);
            }
        }

        // Compute the number of components and the confidence.
        unsigned int components =
            circuit.GetNumConnectedNotNullEntities();
        unsigned int textComponents =
            circuit.GetNumDisconnectedNullEntitiesWithValue();

        unsigned int averageConfidence =
            components != 0 ? confidence/components : 0;
        unsigned int averageTextConfidence =
            textComponents != 0 ? textConfidence/textComponents : 0;

        // Save the best text circuit for later.
        if (textComponents >= m_result.m_textComponents &&
            averageTextConfidence >= m_result.m_textConfidence)
        {
            bestTextCircuit = circuit;
            bestPeaks = peaks;

            m_result.m_textThreshold = threshold;
            m_result.m_textConfidence = averageTextConfidence;
            m_result.m_textComponents = textComponents;
        }

        // Associate text with a circuit component. Go through the peaks again.
        for (unsigned int i = 0; i < bestTextCircuit.GetNumEntities(); i++)
        {
            if (!bestTextCircuit.IsConnected(i) && bestTextCircuit.HasValue(i))
            {
                // Find a component to associate with.
                int best = -1;
                int bestDistance = 0;
                Rectangle rect = bestPeaks.GetVector()[i];

                for (unsigned int j = 0; j < circuit.GetNumEntities(); j++)
                {
                    // Just connect it to the closest valid connected component.
                    // This may connect it to the wrong component in the case
                    // that the correct one has a missing connection. It's not
                    // that bad a failure condition though, because there will
                    // be a corresponding netlist warning.
                    //
                    // I'm doing it's better than adding an extra loop and
                    // finding the "min distance over the min distances" and
                    // adding that one first.
                    if (circuit.IsConnected(j) &&
                        !circuit.HasValue(j) &&
                        !circuit.GetComponentID(j).IsNull())
                    {
                        unsigned int distance =
                            rect.DistanceTo(peaks.GetVector()[j]);

                        if (best == -1 || distance < bestDistance)
                        {
                            best = j;
                            bestDistance = distance;
                        }
                    }
                }

                // Otherwise, just discard the text.
                if (best != -1)
                {
                    circuit.SetValue(best, bestTextCircuit.GetValue(i));
                }
            }
        }

        // Find the netlist.
        NetlistGenerator generator(circuit);
        generator.ConvertToNetlist();

        CHECKPOINT_HAVECONF(averageConfidence, averageTextConfidence);

        // Update the members if this is the best circuit object so far.
        if (components >= m_result.m_components &&
            averageConfidence >= m_result.m_confidence)
        {
            m_result.m_netlist = generator.GetNetlist();
            m_result.m_error = generator.GetErrorText();

            m_result.m_threshold = threshold;
            m_result.m_confidence = averageConfidence;
            m_result.m_components = components;
        }

        CHECKPOINT_HAVECIRCUIT(circuit);
        CHECKPOINT_HAVEGEN(generator);
    }

    CHECKPOINT_DONE();

    // Release working blob image.
    cvReleaseImage(&blob);
}

}
