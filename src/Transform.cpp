// -----------------------------------------------------------------------------
// Implementation of the transform library. Used to map rectangular spaces
// in one coordinate basis to another coordinate basis. Useful for manipulating
// multiple instances of the same images that have been resized.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include "Transform.h"

namespace cs
{

Rectangle Transform::Map(Rectangle rect)
{
    // rect is the source, result is the dst.
    Rectangle result(0, 0, 0, 0);

    // src/dst = xsrc/xdst ==> xdst = xsrc * (dst/src)
    result.SetX(rect.GetX() * ((double) m_dst.GetWidth()/m_src.GetWidth()));

    // And the rest...
    result.SetY(rect.GetY() * ((double) m_dst.GetHeight()/m_src.GetHeight()));

    result.SetWidth(
            rect.GetWidth() * ((double) m_dst.GetWidth()/m_src.GetWidth()));
    result.SetHeight(
            rect.GetHeight() * ((double) m_dst.GetHeight()/m_src.GetHeight()));

    return result;
}

} // end namespace cs
