// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_PEAKFINDER_PEAKFINDER_H
#define API_PEAKFINDER_PEAKFINDER_H

#include <opencv/cv.h>

#include "WhiteoutPeakFinder.h"
#include "PeakVector.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class PeakFinder
// -----------------------------------------------------------------------------

class PeakFinder
{
private:
    // The source image.
    IplImage& m_src;

    // Our whiteout object.
    WhiteoutPeakFinder m_whiteout;

    // The vector of peaks that we will calculate
    PeakVector m_vector;

    // Boolean indicating whether or not we have calculated the peaks yet.
    bool m_done;

public:
    // Constructor. Input is a 3-channel, small, image containing blobs (peaks)
    // that we will find.
    //
    // If the input does not match this descrption, shit will happen with the
    // whiteout object.
    PeakFinder(IplImage& src) :
        m_src(src),
        m_whiteout(src),
        m_vector(Space(src.width, src.height)),
        m_done(false)
    {
        // no-op
    }

    // Do the work.
    PeakVector& FindPeaks(unsigned int scaleFactor);
};

} // end namespace cs

#endif
