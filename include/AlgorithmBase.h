// -----------------------------------------------------------------------------
// This header contains the public prototypes for the algorithm library.
// The algorithm library brings all the other  libraries together; it
// takes an opencv image as an input and the output is a netlist.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_ALGORITHM_ALGORITHMBASE_H
#define API_ALGORITHM_ALGORITHMBASE_H

#include <opencv/cv.h>

#include "AlgorithmResult.h"
#include "ResultCircuits.h"
#include "ResultEnglish.h"
#include "Rectangle.h"
#include "Filter.h"
#include "Circuit.h"
#include "NetlistGenerator.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class AlgorithmBase
// -----------------------------------------------------------------------------

class __attribute__ ((visibility ("default"))) AlgorithmBase
{
protected:
    // -------------------------------------------------------------------------
    // Members
    // -------------------------------------------------------------------------

    // The source images.
    IplImage& m_src;

    // The  algorithm result.
    AlgorithmResult m_result;

    // -------------------------------------------------------------------------
    // Virtual functions (checkpoints)
    // -------------------------------------------------------------------------

    virtual void CHECKPOINT_STARTING(const Filter& filter) = 0;
    virtual void CHECKPOINT_NEWTHRESH(unsigned int threshold) = 0;
    virtual void CHECKPOINT_FOUNDPEAKS(const IplImage& compblob) = 0;
    virtual void CHECKPOINT_FOUNDCONNS(const IplImage& connblob) = 0;

    virtual void CHECKPOINT_TESSERACT(const Rectangle& map) = 0;
    virtual void CHECKPOINT_CIRCUITS(const ResultCircuits& res) = 0;
    virtual void CHECKPOINT_ENGLISH(const ResultEnglish& res) = 0;

    virtual void CHECKPOINT_HAVECIRCUIT(const Circuit& circuit) = 0;
    virtual void CHECKPOINT_HAVEGEN(const NetlistGenerator& g) = 0;
    virtual void CHECKPOINT_HAVECONF(unsigned int conf, unsigned int tconf) = 0;

    virtual void CHECKPOINT_DONE() = 0;

    // -------------------------------------------------------------------------
    // Defaults
    // -------------------------------------------------------------------------

public:
    // Constructor.
    AlgorithmBase(IplImage& src) :
        m_src(src)
    {
        // no-op
    }

    // -------------------------------------------------------------------------
    // Accessors
    // -------------------------------------------------------------------------

    const AlgorithmResult& GetResult() const {return m_result;}

    // Constants are here because we want the command line to have access.
    static unsigned int ThresholdStart() {return 7;}
    static unsigned int ThresholdEnd() {return 18;}

    // -------------------------------------------------------------------------
    // Mutators
    // -------------------------------------------------------------------------

    // Converts the source image to a netlist. May throw a Exception of a
    // cv::Exception, so the user is advised to catch.
    void ConvertToNetlist(
            unsigned int thresholdStart = ThresholdStart(),
            unsigned int thresholdEnd = ThresholdEnd());
};

} // end namespace cs

#endif
