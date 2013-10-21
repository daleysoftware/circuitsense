// -----------------------------------------------------------------------------
// Implementation of the peakfinder library. This specific implementation
// file could also belong in the whiteout lib, but because it is so
// specific I moved it here.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>

#include "WhiteoutPeakFinder.h"

namespace cs
{

Rectangle WhiteoutPeakFinder::WhiteoutPeak(const Point& p)
{
    // Width and height are invalid index values.
    m_topleft = Point(m_src.width, m_src.height);
    m_botright = Point(0, 0);

    DoWhiteout(
            Space(m_src.width, m_src.height),
            Rectangle(p.GetX(), p.GetY(), 0, 0),
            p,
            Mask(0xff, 0xff, 0xff),
            true,
            true);

    if (m_botright.GetX() > m_topleft.GetX() &&
        m_botright.GetY() > m_topleft.GetY())
    {
        return Rectangle(m_topleft.GetX(), m_topleft.GetY(),
                m_botright.GetX() - m_topleft.GetX() + 1,
                m_botright.GetY() - m_topleft.GetY() + 1);
    }

    return Rectangle(0, 0, 0, 0);
}

void WhiteoutPeakFinder::PointFound(const Point& p)
{
    if (p.GetX() < m_topleft.GetX())
    {
        Point newPoint(p.GetX(), m_topleft.GetY());
        m_topleft = newPoint;
    }

    if (p.GetY() < m_topleft.GetY())
    {
        Point newPoint(m_topleft.GetX(), p.GetY());
        m_topleft = newPoint;
    }

    if (p.GetX() > m_botright.GetX())
    {
        Point newPoint(p.GetX(), m_botright.GetY());
        m_botright = newPoint;
    }

    if (p.GetY() > m_botright.GetY())
    {
        Point newPoint(m_botright.GetX(), p.GetY());
        m_botright = newPoint;
    }
}

} // end namespace cs
