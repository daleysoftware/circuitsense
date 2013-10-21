// -----------------------------------------------------------------------------
// Implementation of the connfinder library. This specific implementation
// file could also belong in the whiteout lib, but because it is so
// specific I moved it here.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.h>

#include "WhiteoutConnFinder.h"

namespace cs
{

bool WhiteoutConnFinder::GreyoutConnection(const Point& p)
{
    // Save this for the check at the end of the function, for the return value.
    unsigned int size = m_conns.GetVector().size();

    // Cache the point where the virtual call backs can see it.
    m_graypoint = p;

    // Find the index of the peak we're working off of.
    unsigned int counter = 0;
    m_index = 0;

    for (PeakVector::RectangleVector::const_iterator pk =
            m_peaks.GetVector().begin();
            pk != m_peaks.GetVector().end(); pk++, counter++)
    {
        if (pk->Borders(p))
        {
            m_index = counter;
            break;
        }
    }

    // Clockwise (last param) doesn't matter here.
    DoWhiteout(
            Space(m_src.width, m_src.height),
            Rectangle(p.GetX(), p.GetY(), 0, 0),
            p,
            Mask(0x88, 0x88, 0x88),
            true,
            true);

    // The size change, therefore a connection was added, therefore return true.
    // Could've done this in one line, but this is clearer.
    if (size != m_conns.GetVector().size())
    {
        return true;
    }

    // Otherwise false.
    return false;
}


// Return true when we should stop searching adjacent cells.
bool WhiteoutConnFinder::AdditionalCheck(const Point& p)
{
    // Return true when we are contained within a peak.
    for (PeakVector::RectangleVector::const_iterator pk =
            m_peaks.GetVector().begin();
            pk != m_peaks.GetVector().end(); pk++)
    {
        if (pk->Contains(p))
        {
            return true;
        }
    }

    return false;
}

void WhiteoutConnFinder::PointFound(const Point& p)
{
    // Add a connection if we border a peak and it's not the same point we
    // started out with.
    //
    // Also adds a border so we don't count this connection again.

    unsigned int counter = 0;
    for (PeakVector::RectangleVector::const_iterator pk =
            m_peaks.GetVector().begin();
            pk != m_peaks.GetVector().end(); pk++, counter++)
    {
        if (pk->Borders(p) && p != m_graypoint)
        {
            m_conns.Push(Connection(m_index, counter, m_graypoint, p));

            // Add a red border.
            uint8_t* data = Point::GetDataByPoint(m_src, p);
            data[0] = 0x00; data[1] = 0x00; data[2] = 0x00;
            m_pwhite.RedoutBorder(*pk, p, true);
            data[0] = 0x00; data[1] = 0x00; data[2] = 0x00;
            m_pwhite.RedoutBorder(*pk, p, false);

            // And leave this particular cell blue-green.
            data[0] = 0xff; data[1] = 0xff; data[2] = 0x7f;

            // No need to continue searching.
            return;
        }
    }
}

} // end namespace cs
