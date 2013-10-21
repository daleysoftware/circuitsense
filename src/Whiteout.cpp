// -----------------------------------------------------------------------------
// Author: Matt Pillar
//
// Implementation of the whiteout library. This library is used to whiteout
// the edges of images.
//
// Copyright (c) 2011 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <exception>

#include "Whiteout.h"
#include "Exception.h"
#include "OutsideRectangleIterator.h"

namespace cs
{

Whiteout::Whiteout(IplImage& src) : m_src(src)
{
    if (src.nChannels != 3)
    {
        throw Exception("Whiteout image must be 3 channel");
    }
}

bool Whiteout::WhiteoutHelper(
        bool viral,
        Mask& mask,
        unsigned int& depth,
        PointVector& pv,
        Point p)
{
    // Perform the additional check. Note this is a virtual function.
    if (AdditionalCheck(p))
    {
        // The check says this point is invalid.
        return false;
    }

    // Get the point.
    uint8_t* data = Point::GetDataByPoint(m_src, p);
    if (data == 0)
    {
        // The point is outside the image.
        return false;
    }

    // This will be on the stack. Bad, but no avoiding this.
    Point point(0, 0);

    // If any colour component is less that half intensity, it is considered a
    // "dark" cell, and we will "white" it out.
    if (data[0] < 0x7f || data[1] < 0x7f || data[2] < 0x7f)
    {
        // Maximum stack depth.
        depth++;
        if (depth > 100)
        {
            pv.push_back(p);
            return false;
        }

        // Whiteout this cell.
        data[0] = mask.GetBlue();
        data[1] = mask.GetGreen();
        data[2] = mask.GetRed();

        // Point found callback (virtual).
        PointFound(p);

        if (viral)
        {
            // To the right.
            point = Point(p.GetX()+1, p.GetY());
            WhiteoutHelper(viral, mask, depth, pv, point);

            // To the bottom.
            point = Point(p.GetX(), p.GetY()+1);
            WhiteoutHelper(viral, mask, depth, pv, point);

            // To the left.
            point = Point(p.GetX()-1, p.GetY());
            WhiteoutHelper(viral, mask, depth, pv, point);

            // To the top.
            point = Point(p.GetX(), p.GetY()-1);
            WhiteoutHelper(viral, mask, depth, pv, point);
        }

        return true;
    }

    return false;
}

void Whiteout::DoWhiteout(
        Space space,
        Rectangle rect,
        Point starting,
        Mask mask,
        bool clockwise, bool viral)
{
    unsigned int depth;
    PointVector pv;

    // Loop around the outside of the rectangle. Call whiteout helper at each
    // pixel.
    for (OutsideRectangleIterator it(space, rect, starting); it.More();)
    {
        depth = 0;
        if (!WhiteoutHelper(viral, mask, depth, pv, it.GetCurrent()) &&
            !viral)
        {
            return;
        }

        while (!pv.empty())
        {
            Point vpoint = pv.front();
            pv.pop_front();

            depth = 0;
            if (!WhiteoutHelper(viral, mask, depth, pv, vpoint) && 
                !viral)
            {
                return;
            }
        }

        // Move either clockwise or counter clockwise.
        if (clockwise)
        {
            it++;
        }
        else
        {
            it--;
        }
    }
}

} // end namespace cs
