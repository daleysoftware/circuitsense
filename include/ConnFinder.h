// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CONNFINDER_CONNFINDER_H
#define API_CONNFINDER_CONNFINDER_H

#include <opencv/cv.h>

#include "Whiteout.h"
#include "PeakFinder.h"
#include "ConnVector.h"
#include "WhiteoutConnFinder.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class ConnFinder
// -----------------------------------------------------------------------------

class ConnFinder
{
private:
    // The source image.
    IplImage& m_src;

    // Our whiteout object.
    WhiteoutConnFinder m_cwhite;

    // Perimeter whiteout object
    WhiteoutPerimeter m_pwhite;

    // A reference to the peak vector.
    const PeakVector& m_peaks;

public:
    // Constructor. Input is a 3-channel, small, image containing the conn blob
    // (ususally the output of filter).
    //
    // If the input does not match this descrption, shit will happen with the
    // whiteout object.
    ConnFinder(IplImage& src, const PeakVector& peaks) :
        m_src(src),
        m_cwhite(src, peaks),
        m_pwhite(src),
        m_peaks(peaks)
    {
        // no-op
    }

    // Do the work!
    const ConnVector& GetConnections();
};

} // end namespace cs

#endif
