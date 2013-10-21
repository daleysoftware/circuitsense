// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_WHITEOUTPEAKFINDER_H
#define API_WHITEOUT_WHITEOUTPEAKFINDER_H

#include <stdint.h>
#include <opencv/cv.h>

#include "Rectangle.h"
#include "Point.h"
#include "Whiteout.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class WhiteoutPeakFinder
// -----------------------------------------------------------------------------

class WhiteoutPeakFinder : public Whiteout
{
private:
    // Cache the peak so the callback has it. Store the peak as two points,
    // since holding the peak as the meta would be difficult.
    Point m_topleft;
    Point m_botright;

protected:
    // No additional check.
    virtual bool AdditionalCheck(const Point& p) {return false;}

    // Update the peak if applicable.
    virtual void PointFound(const Point& p);

public:
    // Constructor.
    WhiteoutPeakFinder(IplImage& src) :
        Whiteout(src),
        m_topleft(0, 0),
        m_botright(0, 0)
    {
        // no-op
    }

    // Return the rectangle that holds the peak we just whited out. If the pixel
    // was white in the first place, return a zero size rectangle.
    Rectangle WhiteoutPeak(const Point& p);
};

} // end namespace cs

#endif
