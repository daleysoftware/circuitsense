// -----------------------------------------------------------------------------
// This header contains the public prototypes for the whiteout library,
// which is used to whiteout the edges of images for peak detection.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_POINT_H
#define API_WHITEOUT_POINT_H

#include <stdint.h>
#include <opencv/cv.h>

namespace cs
{

// -----------------------------------------------------------------------------
// class Point
// -----------------------------------------------------------------------------

class Point
{
private:
    unsigned int m_x;
    unsigned int m_y;

public:
    // Constructor.
    Point(unsigned int x, unsigned int y) : m_x(x), m_y(y)
    {
        // no-op
    }

    // Accessors
    unsigned int GetX() const {return m_x;}
    unsigned int GetY() const {return m_y;}
    std::string ToString() const;

    // Mutators
    void SetX(unsigned int x) {m_x = x;}
    void SetY(unsigned int y) {m_y = y;}

    void IncrX() {m_x++;}
    void DecrX() {m_x--;}
    void IncrY() {m_y++;}
    void DecrY() {m_y--;}

    // Statics
    static uint8_t* GetDataByPoint(IplImage& img, const Point& p);

    // Operator overloads
    bool operator==(const Point& p) const;
    bool operator!=(const Point& p) const;
};

} // end namespace cs

#endif
