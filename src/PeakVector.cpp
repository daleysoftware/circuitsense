// -----------------------------------------------------------------------------
// Implementation of the peakfinder library. Used to map rectangular spaces
// in one coordinate basis to another coordinate basis. Useful for manipulating
// multiple instances of the same images that have been resized.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <sstream>

#include "PeakVector.h"

namespace cs
{

std::string PeakVector::ToString() const
{
    if (m_peaks.size() == 0)
    {
        return "{}";
    }

    std::ostringstream os;

    for (RectangleVector::const_iterator it = m_peaks.begin();
            it != m_peaks.end(); it++)
    {
        os << it->ToString() << ",";
    }

    return os.str().substr(0, os.str().size()-1);
}

void PeakVector::Scale(double factor)
{
    for (RectangleVector::iterator it = m_peaks.begin();
            it != m_peaks.end(); it++)
    {
        it->Scale(factor);

        // Fix the width if it's too big.
        if (it->GetX() + it->GetWidth() + 1 >= m_space.GetWidth())
        {
            it->SetWidth(m_space.GetWidth() - it->GetX() - 1);
        }

        // Fix the height if it's too big.
        if (it->GetY() + it->GetHeight() + 1 >= m_space.GetHeight())
        {
            it->SetHeight(m_space.GetHeight() - it->GetY() - 1);
        }
    }
}

unsigned int PeakVector::GetAverageArea() const
{
    if (m_peaks.size() == 0)
    {
        return 0;
    }

    unsigned int sum = 0;
    for (RectangleVector::const_iterator it = m_peaks.begin();
            it != m_peaks.end(); it++)
    {
        sum += it->GetArea();
    }

    return sum/m_peaks.size();
}

} // end namespace cs
