// -----------------------------------------------------------------------------
// This particular class is used to iterator over the outside of a rectangle.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_SPACEITERATOR_H
#define API_WHITEOUT_SPACEITERATOR_H

#include <opencv/cv.h>

#include "Space.h"
#include "Point.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class SpaceIterator
// -----------------------------------------------------------------------------

class SpaceIterator
{
private:
    // The space that we are working with.
    Space m_space;

    // Boolean indicating whether we are done or not.
    bool m_done;

    // Our current point.
    Point m_current;

    // Our starting point.
    Point m_starting;

public:
    // Constructor.
    SpaceIterator(Space space);

    // Returns true when we are done iterating.
    bool Done() const {return m_done;}

    // The opposite of Done().
    bool More() const {return !m_done;}

    // Go to the next point.
    void operator++(int);

    // Return the Point we are currently at.
    const Point& GetCurrent() const {return m_current;}
};

} // end namespace cs

#endif
