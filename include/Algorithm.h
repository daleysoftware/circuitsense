// -----------------------------------------------------------------------------
// Note that I'm really just stubbing out virtual functions here. The only
// reason for doing this over making them non-pure in the base class is this
// has the added advantage of failing to compile if the other derived classes
// do not implement one of the callback functions.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_ALGORITHM_ALGORITHM_H
#define API_ALGORITHM_ALGORITHM_H

#include <opencv/cv.h>
#include "AlgorithmBase.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class Algorithm
// -----------------------------------------------------------------------------

class Algorithm : public AlgorithmBase
{
protected:
    // -------------------------------------------------------------------------
    // Virtual functions
    // -------------------------------------------------------------------------

    virtual void CHECKPOINT_STARTING(const Filter& filter) {}
    virtual void CHECKPOINT_NEWTHRESH(unsigned int threshold) {}
    virtual void CHECKPOINT_FOUNDPEAKS(const IplImage& compblob) {}
    virtual void CHECKPOINT_FOUNDCONNS(const IplImage& connblob) {}

    virtual void CHECKPOINT_TESSERACT(const Rectangle& map) {}
    virtual void CHECKPOINT_CIRCUITS(const ResultCircuits& res) {}
    virtual void CHECKPOINT_ENGLISH(const ResultEnglish& res) {}

    virtual void CHECKPOINT_HAVECIRCUIT(const Circuit& circuit) {}
    virtual void CHECKPOINT_HAVEGEN(const NetlistGenerator& g) {}
    virtual void CHECKPOINT_HAVECONF(unsigned int conf, unsigned int tconf) {}

    virtual void CHECKPOINT_DONE() {}

public:
    // Constructor.
    Algorithm(IplImage& src) : AlgorithmBase(src)
    {
        // no-op
    }
};

} // end namespace cs

#endif
