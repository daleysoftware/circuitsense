// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_PEAKFINDER_PEAKVECTOR_H
#define API_PEAKFINDER_PEAKVECTOR_H

#include <string>
#include <vector>
#include <opencv/cv.h>

#include "Rectangle.h"
#include "Space.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class PeakVector
// -----------------------------------------------------------------------------

class PeakVector
{
public:
    typedef std::vector<Rectangle> RectangleVector;

private:
    // The vector of peaks, stored as rectangles.
    RectangleVector m_peaks;

    // The  space that contains these peaks.
    Space m_space;

public:
    PeakVector(Space space) : m_space(space)
    {
        // no-op
    }

    // Mutators
    void Push(Rectangle& rect) {m_peaks.push_back(rect);}
    void Scale(double factor);

    // Accessors
    const RectangleVector& GetVector() const{return m_peaks;}
    std::string ToString() const;
    unsigned int GetAverageArea() const;
};

} // end namespace cs

#endif
