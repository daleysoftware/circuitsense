// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "WhiteoutPerimeter.h"

namespace cs
{

void WhiteoutPerimeter::DoWhiteoutPerimeter()
{
    // Recursive clockwise.
    DoWhiteout(
            Space(m_src.width, m_src.height),
            Rectangle(0, 0, m_src.width-1, m_src.height-1),
            Point(0, 0),
            Mask(0xff, 0xff, 0xff),
            true,
            true);
}

void WhiteoutPerimeter::BlueoutBorder(Rectangle rect,
        Point starting, bool clockwise)
{
    // Not viral.
    DoWhiteout(
            Space(m_src.width, m_src.height),
            rect,
            starting,
            Mask(0xff, 0x7f, 0x7f),
            clockwise,
            false);
}

void WhiteoutPerimeter::GreenoutBorder(Rectangle rect,
        Point starting, bool clockwise)
{
    // Not viral.
    DoWhiteout(
            Space(m_src.width, m_src.height),
            rect,
            starting,
            Mask(0x7f, 0xff, 0x7f),
            clockwise,
            false);
}

void WhiteoutPerimeter::RedoutBorder(Rectangle rect,
        Point starting, bool clockwise)
{
    // Not viral.
    DoWhiteout(
            Space(m_src.width, m_src.height),
            rect,
            starting,
            Mask(0x7f, 0x7f, 0xff),
            clockwise,
            false);
}

} // end namespace cs
