// -----------------------------------------------------------------------------
// Implementation of the transform library. Used to map rectangular spaces
// in one coordinate basis to another coordinate basis. Useful for manipulating
// multiple instances of the same images that have been resized.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>

#include "Point.h"
#include "Transform.h"

namespace cs
{

bool Rectangle::Contains(const Point& p) const
{
    if (p.GetX() > GetX() &&
        p.GetY() > GetY() &&
        p.GetX() < GetX() + GetWidth() &&
        p.GetY() < GetY() + GetHeight())
    {
        return true;
    }

    return false;
}

bool Rectangle::Borders(const Point& p) const
{
    // It's in the big rectangle and it doesn't contain the point, so it must
    // border the point.
    if (p.GetX() >= GetX() &&
        p.GetY() >= GetY() &&
        p.GetX() <= GetX() + GetWidth() &&
        p.GetY() <= GetY() + GetHeight() &&
        !Contains(p))
    {
        return true;
    }

    return false;
}

unsigned int Rectangle::DistanceTo(const Rectangle& rect) const
{
    double myXMiddle = GetX() + GetWidth()/2.0;
    double myYMiddle = GetY() + GetHeight()/2.0;

    double hisXMiddle = rect.GetX() + rect.GetWidth()/2.0;
    double hisYMiddle = rect.GetY() + rect.GetHeight()/2.0;

    double xDiff = hisXMiddle - myXMiddle;
    double yDiff = hisYMiddle - myYMiddle;

    return (unsigned int) sqrt(xDiff*xDiff + yDiff*yDiff);
}

std::string Rectangle::ToString() const
{
    std::ostringstream os;

    os
        << "{x=" << m_x << ",y=" << m_y
        << ",width=" << m_width << ",height=" << m_height << "}";

    return os.str();
}

void Rectangle::Scale(double factor)
{
    Rectangle rect = *this;

    m_width = (unsigned int) (GetWidth() * factor);
    m_height = (unsigned int) (GetHeight() * factor);

    unsigned int subWidth = (GetWidth() - rect.GetWidth()) / 2;
    unsigned int subHeight = (GetHeight() - rect.GetHeight()) / 2;

    if (subWidth > rect.m_x)
    {
        m_x = 0;
    }
    else
    {
        m_x = rect.m_x - subWidth;
    }

    if (subHeight > rect.m_y)
    {
        m_y = 0;
    }
    else
    {
        m_y = rect.m_y - subHeight;
    }
}

void Rectangle::Merge(const Rectangle& rect)
{
    Rectangle newRect(0,0,0,0);

    newRect.m_x = std::min(GetX(), rect.GetX());
    newRect.m_y = std::min(GetY(), rect.GetY());

    unsigned int rightX, rightY;

    rightX = std::max(GetX()+GetWidth(), rect.GetX()+rect.GetWidth());
    rightY = std::max(GetY()+GetHeight(), rect.GetY()+rect.GetHeight());

    newRect.m_width = rightX-newRect.GetX();
    newRect.m_height = rightY-newRect.GetY();

    *this = newRect;
}

} // end namespace cs
