// -----------------------------------------------------------------------------
// Implementation of the debug library. This library is used to bring debug
// the main algorithm library. This is the only  api library that will
// make opencv highgui calls. It is not linked in the final product.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "AlgorithmTimer.h"
#include "TimeAnalyzer.h"

namespace cs
{

void AlgorithmTimer::CHECKPOINT_STARTING(const Filter& filter)
{
    PushMessage("'s Algorithm");
}

void AlgorithmTimer::CHECKPOINT_NEWTHRESH(unsigned int threshold)
{
    std::ostringstream os1;
    os1 << "Threshold " << threshold << "...";
    PushMessage(os1.str());

    PushMessage("Comp filter, whiteout, peakfinder");
}

void AlgorithmTimer::CHECKPOINT_FOUNDPEAKS(const IplImage& compblob)
{
    PopMessage();
    PushMessage("Conn filter, whiteout, scale, connfinder");
}

void AlgorithmTimer::CHECKPOINT_FOUNDCONNS(const IplImage& connblob)
{
    PopMessage();
    PushMessage("Apply tesseract, associate components");
}

void AlgorithmTimer::CHECKPOINT_TESSERACT(const Rectangle& map)
{
    // no-op
}

void AlgorithmTimer::CHECKPOINT_CIRCUITS(const ResultCircuits& res)
{
    // no-op
}

void AlgorithmTimer::CHECKPOINT_ENGLISH(const ResultEnglish& res)
{
    // no-op
}

void AlgorithmTimer::CHECKPOINT_HAVECIRCUIT(const Circuit& circuit)
{
    PopMessage();
}

void AlgorithmTimer::CHECKPOINT_HAVEGEN(const NetlistGenerator& g)
{
    PopMessage();
}

void AlgorithmTimer::CHECKPOINT_HAVECONF(unsigned int conf,
        unsigned int tconf)
{
    // no-op
}

void AlgorithmTimer::CHECKPOINT_DONE()
{
    PopMessage();
    PrintAnalysis();
}

} // end namespace cs
