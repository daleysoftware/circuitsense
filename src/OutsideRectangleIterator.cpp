// -----------------------------------------------------------------------------
// Implementation of the whiteout library. This library is used to whiteout
// the edges of images.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include <assert.h>

#include "OutsideRectangleIterator.h"
#include "Whiteout.h"

namespace cs
{

OutsideRectangleIterator::OutsideRectangleIterator(
        Space space,
        Rectangle rectangle,
        Point starting) :
    m_space(space),
    m_rectangle(rectangle),
    m_done(false),
    m_current(starting.GetX(), starting.GetY()),
    m_starting(starting)
{
    // Space cannot be zero.
    if (space.GetWidth() == 0 || space.GetHeight() == 0)
    {
        throw Exception("Space cannot be zero");
    }

    // Is the rectangle within the space?
    if (rectangle.GetX() + rectangle.GetWidth() >= space.GetWidth() ||
        rectangle.GetY() + rectangle.GetHeight() >= space.GetHeight())
    {
        std::ostringstream os;
        os
            << "Rectangle " << rectangle.ToString()
            << " is not within space " << space.ToString();

        throw Exception(os.str());
    }
}

void OutsideRectangleIterator::operator++(int)
{
    // Special handling for the zero case.
    if (m_rectangle.IsZero())
    {
        m_done = true;
        return;
    }

    if (Done())
    {
        return;
    }

    // Top line.
    if (m_current.GetY() == m_rectangle.GetY())
    {
        if (m_current.GetX() == m_rectangle.GetX() + m_rectangle.GetWidth())
        {
            // Top right corner.
            m_current.IncrY();
            goto outside_rectangle_iterator_plusplus_done;
        }
        else
        {
            // Top line.
            m_current.IncrX();
            goto outside_rectangle_iterator_plusplus_done;
        }
    }

    // Right line.
    if (m_current.GetX() == m_rectangle.GetX() + m_rectangle.GetWidth())
    {
        if (m_current.GetY() == m_rectangle.GetY() + m_rectangle.GetHeight())
        {
            // Bottom right corner.
            m_current.DecrX();
            goto outside_rectangle_iterator_plusplus_done;
        }
        else
        {
            // Right line.
            m_current.IncrY();
            goto outside_rectangle_iterator_plusplus_done;
        }
    }

    // Bottom line.
    if (m_current.GetY() == m_rectangle.GetY() + m_rectangle.GetHeight())
    {
        if (m_current.GetX() == m_rectangle.GetX())
        {
            // Bottom right corner.
            m_current.DecrY();
            goto outside_rectangle_iterator_plusplus_done;
        }
        else
        {
            // Bottom line.
            m_current.DecrX();
            goto outside_rectangle_iterator_plusplus_done;
        }
    }

    // Left line.
    if (m_current.GetX() == m_rectangle.GetX())
    {
        if (m_current.GetY() == m_rectangle.GetY())
        {
            // Top left corner. We should never reach here because of the top
            // line else.
            assert(0);
        }
        else
        {
            // Left line.
            m_current.DecrY();
            // goto not required.
        }
    }

outside_rectangle_iterator_plusplus_done:

    // Process this block before exiting.
    if (m_current == m_starting)
    {
        m_done = true;
    }
}

void OutsideRectangleIterator::operator--(int)
{
    // Special handling for the zero case.
    if (m_rectangle.IsZero())
    {
        m_done = true;
        return;
    }

    if (Done())
    {
        return;
    }

    // Left line.
    if (m_current.GetX() == m_rectangle.GetX())
    {
        if (m_current.GetY() == m_rectangle.GetY() + m_rectangle.GetHeight())
        {
            // Bottom left corner.
            m_current.IncrX();
            goto outside_rectangle_iterator_minusminus_done;
        }
        else
        {
            // Left line.
            m_current.IncrY();
            goto outside_rectangle_iterator_minusminus_done;
        }
    }

    // Bottom line.
    if (m_current.GetY() == m_rectangle.GetY() + m_rectangle.GetHeight())
    {
        if (m_current.GetX() == m_rectangle.GetX() + m_rectangle.GetWidth())
        {
            // Bottom right corner.
            m_current.DecrY();
            goto outside_rectangle_iterator_minusminus_done;
        }
        else
        {
            // Bottom line.
            m_current.IncrX();
            goto outside_rectangle_iterator_minusminus_done;
        }
    }

    // Right line.
    if (m_current.GetX() == m_rectangle.GetX() + m_rectangle.GetWidth())
    {
        if (m_current.GetY() == m_rectangle.GetY())
        {
            // Top right corner.
            m_current.DecrX();
            goto outside_rectangle_iterator_minusminus_done;
        }
        else
        {
            // Right line.
            m_current.DecrY();
            goto outside_rectangle_iterator_minusminus_done;
        }
    }

    // Top line.
    if (m_current.GetY() == m_rectangle.GetY())
    {
        if (m_current.GetX() == m_rectangle.GetX())
        {
            // Top left corner. We should never reach here because of the left
            // line else.
            assert(0);
        }
        else
        {
            // Top line.
            m_current.DecrX();
            // goto not required.
        }
    }

outside_rectangle_iterator_minusminus_done:

    // Process this block before exiting.
    if (m_current == m_starting)
    {
        m_done = true;
    }
}

} // end namespace cs
