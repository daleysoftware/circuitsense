// -----------------------------------------------------------------------------
// This library is used to transform rectangular spaces in one basis to another.
// Similar to an image definition, a  space of 2x2 looks like this:
//
// x,x
// x,x
//
// See  rectnagle for more discussion on the semantics of spaces and
// rectangles.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TRANSFORM_SPACE_H
#define API_TRANSFORM_SPACE_H

#include <string>
#include <sstream>

namespace cs
{

class Transform;

// -----------------------------------------------------------------------------
// class Space
// -----------------------------------------------------------------------------

class Space
{
private:
    unsigned int m_width;
    unsigned int m_height;

public:
    Space(unsigned int width, unsigned int height) :
        m_width(width),
        m_height(height)
    {
        // no-op
    }

    // Accessors
    unsigned int GetWidth() const {return m_width;}
    unsigned int GetHeight() const {return m_height;}

    std::string ToString() const
    {
        std::ostringstream os;
        os << "{width=" << m_width << ",height=" << m_height << "}";
        return os.str();
    }
};

} // end namespace cs

#endif
