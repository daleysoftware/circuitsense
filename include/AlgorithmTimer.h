// -----------------------------------------------------------------------------
// This header contains the public prototypes for the debug library. This
// library is used to debug the main algorithm library.
//
// Copyright (c) 2011 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_DEBUG_ALGORITHMTIMER_H
#define API_DEBUG_ALGORITHMTIMER_H

#include <opencv/cv.h>
#include "Algorithm.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class AlgorithmTimer
// -----------------------------------------------------------------------------

class AlgorithmTimer : public Algorithm
{
protected:
    // Defined as timing functions.

    virtual void CHECKPOINT_STARTING(const Filter& filter);
    virtual void CHECKPOINT_NEWTHRESH(unsigned int threshold);
    virtual void CHECKPOINT_FOUNDPEAKS(const IplImage& compblob);
    virtual void CHECKPOINT_FOUNDCONNS(const IplImage& connblob);

    virtual void CHECKPOINT_TESSERACT(const Rectangle& map);
    virtual void CHECKPOINT_CIRCUITS(const ResultCircuits& res);
    virtual void CHECKPOINT_ENGLISH(const ResultEnglish& res);

    virtual void CHECKPOINT_HAVECIRCUIT(const Circuit& circuit);
    virtual void CHECKPOINT_HAVEGEN(const NetlistGenerator& g);
    virtual void CHECKPOINT_HAVECONF(unsigned int conf, unsigned int tconf);

    virtual void CHECKPOINT_DONE();

public:
    // Constructor.
    AlgorithmTimer(IplImage& src) : Algorithm(src)
    {
        // no-op
    }
};

} // end namespace cs

#endif
