// -----------------------------------------------------------------------------
// This header contains the public prototypes for the whiteout library,
// which is used to whiteout the edges of images for peak detection.
//
// This particular class is used to iterator over the outside of a rectangle.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_OUTSIDERECTANGLEITERATOR_H
#define API_WHITEOUT_OUTSIDERECTANGLEITERATOR_H

#include <opencv/cv.h>

#include "Rectangle.h"
#include "Space.h"
#include "Exception.h"
#include "Point.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class OutsideRectangleIterator
// -----------------------------------------------------------------------------

class OutsideRectangleIterator
{
private:
    // The space that contains the rectangle.
    Space m_space;

    // The rectangle we are going to iterate over.
    Rectangle m_rectangle;

    // Boolean indicating whether we are done or not.
    bool m_done;

    // Our current point.
    Point m_current;

    // Our starting point.
    Point m_starting;

public:
    // Constructor. Will throw if the space or rectanle is invalid or if the
    // rectangle is not within the space.
    OutsideRectangleIterator(
            Space space,
            Rectangle rectangle,
            Point starting);

    // Returns true when we are done iterating the outside of the rectangle and
    // false otherwise.
    bool Done() const {return m_done;}

    // The opposite of Done().
    bool More() const {return !m_done;}

    // Go to the next point (clockwise).
    void operator++(int);

    // Go the the previous point (counter-clockwise).
    void operator--(int);

    // Return the Point we are currently at.
    const Point& GetCurrent() const {return m_current;}
};

} // end namespace cs

#endif
