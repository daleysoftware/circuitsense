// -----------------------------------------------------------------------------
// This header contains the public prototypes for the transform library.
// This library is used to transform rectangular spaces in one basis to another.
//
// Examples of  rectangles:
//
// size 0x0: single point at x,y
// size 1x1: x,x
//           x,x
// size 2x2: x,x,x
//           x,x,x
//           x,x,x
// 
// Note that the 'width' and the 'height' represent the number of cells we
// "jump" across. That is why the 1x1 cell is actually a  space of 2x2.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TRANSFORM_RECTANGLE_H
#define API_TRANSFORM_RECTANGLE_H

#include <string>
#include <sstream>

namespace cs
{

// Forward declarations.
class Point;
class Transform;

// -----------------------------------------------------------------------------
// class Rectangle
// -----------------------------------------------------------------------------

class Rectangle
{
private:
    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_width;
    unsigned int m_height;

public:
    Rectangle(
            unsigned int x, unsigned int y,
            unsigned int width, unsigned int height) :
        m_x(x),
        m_y(y),
        m_width(width),
        m_height(height)
    {
        // no-op
    }

    // Accessors.
    std::string ToString() const;

    // True when the point is in the rectangle.
    bool Contains(const Point& p) const;

    // True when the point is on the border of the rectangle.
    bool Borders(const Point& p) const;

    // More accrssors
    unsigned int GetX() const {return m_x;}
    unsigned int GetY() const {return m_y;}
    unsigned int GetWidth() const {return m_width;}
    unsigned int GetHeight() const {return m_height;}
    unsigned int GetArea() const {return m_width*m_height;}

    bool IsZero() const {return m_width == 0 && m_height == 0;}
    unsigned int DistanceTo(const Rectangle& rect) const;

    // Mutators
    void Scale(double factor);

    void SetX(unsigned int x) {m_x = x;}
    void SetY(unsigned int y) {m_y = y;}
    void SetWidth(unsigned int width) {m_width = width;}
    void SetHeight(unsigned int height) {m_height = height;}

    void Merge(const Rectangle& rect);

    void IncrX() {m_x++;}
    void DecrX() {m_x--;}
    void IncrY() {m_y++;}
    void DecrY() {m_y--;}
};

} // end namespace cs

#endif
