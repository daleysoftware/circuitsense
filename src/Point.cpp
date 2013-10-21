// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <exception>
#include <vector>

#include "Whiteout.h"

namespace cs
{

uint8_t* Point::GetDataByPoint(IplImage& img, const Point& p)
{
    // Return a null pointer when the point is not in the image.
    if (p.GetX() >= img.width || p.GetY() >= img.height)
    {
        return 0;
    }

    // Very important to know how the data is indexed.
    uint8_t* data;
    data = (uint8_t*) img.imageData;

    int index = p.GetY()*img.widthStep + p.GetX()*img.nChannels;
    return &data[index];
}

std::string Point::ToString() const
{
    std::ostringstream os;
    os << "{" <<  m_x << "," << m_y << "}";
    return os.str();
}

bool Point::operator==(const Point& p) const
{
    return p.GetX() == GetX() && p.GetY() == GetY();
}

bool Point::operator!=(const Point& p) const
{
    return !operator==(p);
}

}
