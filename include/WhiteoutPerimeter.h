// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_WHITEOUTPERIMETER_H
#define API_WHITEOUT_WHITEOUTPERIMETER_H

#include <stdint.h>
#include <opencv/cv.h>

#include "Whiteout.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class WhiteoutPerimeter
// -----------------------------------------------------------------------------

class WhiteoutPerimeter : public Whiteout
{
protected:
    // No additonal check
    virtual bool AdditionalCheck(const Point& p) {return false;}

    // Nothing to do when a point is found.
    virtual void PointFound(const Point& p) {}

public:
    // Constructor.
    WhiteoutPerimeter(IplImage& src) : Whiteout(src)
    {
        // no-op
    }

    // Functions
    void DoWhiteoutPerimeter(); // This is viral.

    // Not viral. Will stop at a white cell.
    void BlueoutBorder(
            Rectangle rect, Point starting, bool clockwise);
    void GreenoutBorder(
            Rectangle rect, Point starting, bool clockwise);
    void RedoutBorder(
            Rectangle rect, Point starting, bool clockwise);
};

} // end namespace cs

#endif
