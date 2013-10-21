// -----------------------------------------------------------------------------
// This library is used to transform rectangular spaces in one basis to another.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TRANSFORM_TRANSFORM_H
#define API_TRANSFORM_TRANSFORM_H

#include "Space.h"
#include "Rectangle.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class Transform
// -----------------------------------------------------------------------------

class Transform
{
private:
    Space m_src;
    Space m_dst;

public:
    Transform(Space src, Space dst) :
        m_src(src),
        m_dst(dst)
    {
        // no-op
    }

    // Map the source rectangle rect from the src space to the dst space.
    Rectangle Map(Rectangle rect);
};

} // end namespace cs

#endif
