// -----------------------------------------------------------------------------
// Implementation of the connfinder library. This particular class iterates
// over the set of peaks found by a different module, and looks for connections.
// Metadata is stored in the source image to help us know where we're at.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>

#include "Whiteout.h"
#include "ConnVector.h"
#include "ConnFinder.h"
#include "OutsideRectangleIterator.h"

namespace cs
{

const ConnVector& ConnFinder::GetConnections()
{
    // Start by looping through each peak.
    for (PeakVector::RectangleVector::const_iterator pk =
            m_peaks.GetVector().begin();
            pk != m_peaks.GetVector().end(); pk++)
    {
        // Iterate over the outside of the rectangle. Start at the top right 
        // corner for the 3 o'clock port numbering convention.
        for (OutsideRectangleIterator rit(
                    Space(m_src.width, m_src.height),
                    (*pk),
                    Point(pk->GetX()+pk->GetWidth(), pk->GetY()));
                rit.More(); rit++)
        {
            // If it's a dark cell...
            uint8_t* data = Point::GetDataByPoint(m_src, rit.GetCurrent());
            if (data[0] < 0x7f || data[1] < 0x7f || data[2] < 0x7f)
            {
                // Add a green border to this edge (green "from" red "to").
                m_pwhite.GreenoutBorder(*pk, rit.GetCurrent(), true);
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00;
                m_pwhite.GreenoutBorder(*pk, rit.GetCurrent(), false);
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00;

                // Find the connections associated with this cell.
                m_cwhite.GreyoutConnection(rit.GetCurrent());

                // Leave it blue, not gray.
                data[0] = 0xff; data[1] = 0x7f; data[2] = 0x7f;
            }
        }
    }

    // Return what is currently cached in the cwhite object.
    return m_cwhite.GetConnections();
}

} // end namespace cs
