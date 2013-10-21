// -----------------------------------------------------------------------------
// This header contains the public prototypes for the algorithm library.
// This class is a wrapper for the base  algorithm class that includes some
// threading functionality.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_ALGORITHM_ALGORITHMTHREADED_H
#define API_ALGORITHM_ALGORITHMTHREADED_H

#include <opencv/cv.h>
#include <threadmaster/Thread.h>

#include "Exception.h"
#include "AlgorithmBase.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class AlgorithmThreaded
// -----------------------------------------------------------------------------

class __attribute__ ((visibility("default"))) AlgorithmThreaded :
    public AlgorithmBase,
    public ThreadMaster::Thread
{
private:
    // Checks to see if we have been told to stop. If we have, then throw a
    // TerminatedException to stop the thread.
    void CheckStop();

protected:

    // From TheadMaster::Thread
    virtual void Run(void* ptr);

    // -------------------------------------------------------------------------
    // Virtual functions
    // -------------------------------------------------------------------------

    virtual void CHECKPOINT_STARTING(const Filter& filter);
    virtual void CHECKPOINT_NEWTHRESH(unsigned int threshold);
    virtual void CHECKPOINT_FOUNDPEAKS(const IplImage& compblob);
    virtual void CHECKPOINT_FOUNDCONNS(const IplImage& connblob);

    virtual void CHECKPOINT_TESSERACT(const Rectangle& map);
    virtual void CHECKPOINT_CIRCUITS(const ResultCircuits& res);
    virtual void CHECKPOINT_ENGLISH(const ResultEnglish& res);

    virtual void CHECKPOINT_HAVECIRCUIT(const Circuit& circuit);
    virtual void CHECKPOINT_HAVEGEN(const NetlistGenerator& g);
    virtual void CHECKPOINT_HAVECONF(unsigned int conf, unsigned int text);

    virtual void CHECKPOINT_DONE();

public:
    // Constructor.
    AlgorithmThreaded(IplImage& src) :
        AlgorithmBase(src),
        ThreadMaster::Thread()
    {
        // no-op
    }
};

} // end namespace cs

#endif
