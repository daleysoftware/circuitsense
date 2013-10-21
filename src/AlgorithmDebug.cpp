// -----------------------------------------------------------------------------
// Author: Matt Pillar
//
// Implementation of the debug library. This library is used to bring debug
// the main algorithm library. This is the only  api library that will
// make opencv highgui calls. It is not linked in the final product.
//
// Copyright (c) 2011 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "AlgorithmDebug.h"

namespace cs
{

void AlgorithmDebug::CHECKPOINT_STARTING(const Filter& filter)
{
    m_logger << "Starting 's algorithm...";
}

void AlgorithmDebug::CHECKPOINT_NEWTHRESH(unsigned int threshold)
{
    std::ostringstream os1;
    os1 << "Threshold " << threshold << "...";

    std::ostringstream os2;
    os2 << "Evaluate threshold " << threshold << "...";
    m_logger << os2.str();

    m_debug.push_back(DebugRow());
    m_debug.back().m_threshold = threshold;
}

void AlgorithmDebug::CHECKPOINT_FOUNDPEAKS(const IplImage& compblob)
{
    std::ostringstream os;
    os << m_debug.back().m_threshold << "_compblob.jpg";
    cvSaveImage(os.str().c_str(), &compblob);
}

static unsigned int g_tesscounter = 0;
void AlgorithmDebug::CHECKPOINT_FOUNDCONNS(const IplImage& connblob)
{
    std::ostringstream os;
    os << m_debug.back().m_threshold << "_connblob.png";
    cvSaveImage(os.str().c_str(), &connblob);

    g_tesscounter = 0;
}

void AlgorithmDebug::CHECKPOINT_TESSERACT(const Rectangle& map)
{
    std::ostringstream os1;
    os1 << "(" << g_tesscounter << ") Applying tesseract on " << map.ToString();
    m_logger << os1.str();

    cvSetImageROI(&m_src, cvRect(map.GetX(), map.GetY(), map.GetWidth(),
                map.GetHeight()));

    std::ostringstream os2;
    os2 << m_debug.back().m_threshold << "_debug_" << g_tesscounter << ".jpg";
    cvSaveImage(os2.str().c_str(), &m_src);
    cvResetImageROI(&m_src);

    g_tesscounter++;
}

void AlgorithmDebug::CHECKPOINT_CIRCUITS(const ResultCircuits& res)
{
    std::ostringstream os;
    os
        << "Found circuit component {" << res.ToString()
        << "} (confidence=" << res.GetConfidence() << ")";
    m_logger << os.str();
}

void AlgorithmDebug::CHECKPOINT_ENGLISH(const ResultEnglish& res)
{
    std::ostringstream os;
    os
        << "Found text {" << res.GetChars() << "}"
        << " (confidence=" << res.GetConfidence() << ")";
    m_logger << os.str();
}

void AlgorithmDebug::CHECKPOINT_HAVECIRCUIT(const Circuit& circuit)
{
    std::cout << circuit.ToString() << std::endl;

    std::ostringstream os;
    os
        << "We have " << circuit.GetNumConnectedNotNullEntities()
        << " circuit components";
    m_logger << os.str();

    m_debug.back().m_components =
        circuit.GetNumConnectedNotNullEntities();
    m_debug.back().m_textComponents =
        circuit.GetNumDisconnectedNullEntitiesWithValue();
}

void AlgorithmDebug::CHECKPOINT_HAVEGEN(const NetlistGenerator& g)
{
    m_logger << "Netlist:";
    std::cout << g.GetNetlist() << std::endl;

    if (g.HasErrorText())
    {
        m_logger << "Netlist warnings:";
        std::cout << g.GetErrorText() << std::endl;
        m_logger << "Modified circuit:";
        std::cout << g.GetModifiedCircuit().ToString() << std::endl;
    }
}

void AlgorithmDebug::CHECKPOINT_HAVECONF(unsigned int conf,
        unsigned int tconf)
{
    std::ostringstream os;
    os << "conf=" << conf << " tconf=" << tconf;
    m_logger << os.str();

    m_debug.back().m_confidence = conf;
    m_debug.back().m_textConfidence = tconf;
}

void AlgorithmDebug::CHECKPOINT_DONE()
{
    // Print the debug table.
    m_logger << "Debug table:";

    std::cout
        << "threshold,confidence,components,textConfidence,textComponents"
        << std::endl;

    for (std::vector<DebugRow>::const_iterator it = m_debug.begin();
            it != m_debug.end(); it++)
    {
        std::cout << it->ToString() << std::endl;
    }

    std::ostringstream os;
    os
        << "Done. The best circuit and text thresholds are "
        << m_result.GetBestThreshold() << " and "
        << m_result.GetBestTextThreshold() << " respectively.";

    m_logger << os.str();
}

} // end namespace cs
