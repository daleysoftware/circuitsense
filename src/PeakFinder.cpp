// -----------------------------------------------------------------------------
// Implementation of the peakfinder library. Used to map rectangular spaces
// in one coordinate basis to another coordinate basis. Useful for manipulating
// multiple instances of the same images that have been resized.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>

#include "SpaceIterator.h"
#include "PeakFinder.h"

namespace cs
{

PeakVector& PeakFinder::FindPeaks(unsigned int scaleFactor)
{
    if (m_done)
    {
        return m_vector;
    }

    for (SpaceIterator it(Space(m_src.width, m_src.height));
            it.More(); it++)
    {
        Rectangle peak = m_whiteout.WhiteoutPeak(it.GetCurrent());

        if (peak.IsZero())
        {
            continue;
        }

        m_vector.Push(peak);
    }

    return m_vector;
}

} // end namespace cs
