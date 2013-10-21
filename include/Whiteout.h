// -----------------------------------------------------------------------------
// This header contains the public prototypes for the whiteout library,
// which is used to whiteout the edges of images for peak detection.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_WHITEOUT_H
#define API_WHITEOUT_WHITEOUT_H

#include <list>
#include <opencv/cv.h>
#include <stdint.h>

#include "Point.h"
#include "Mask.h"
#include "Transform.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class Whiteout
// -----------------------------------------------------------------------------

class Whiteout
{
protected:
    // Typedefs
    typedef std::list<Point> PointVector;

    // The image source.
    IplImage& m_src;

    // Return true when the cell is not white.
    bool WhiteoutHelper(
            bool viral,
            Mask& mask,
            unsigned int& depth,
            PointVector& pv,
            Point p);

    // Pure virtual functions.
    virtual bool AdditionalCheck(const Point& p) = 0; // True if we should exit.
    virtual void PointFound(const Point& p) = 0;

    // Main whiteout function.
    void DoWhiteout(
            Space space,
            Rectangle rect,
            Point starting,
            Mask mask,
            bool clockwise,
            bool viral);

public:
    // Constructor. The input image source must be a 3-channel image. This will
    // throw if this is not the case.
    //
    // Normally this image is a small grayscale image that has been converted to
    // colour.
    Whiteout(IplImage& src);
};

} // end namespace cs

#endif
