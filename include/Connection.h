// -----------------------------------------------------------------------------
// This header contains the public prototypes for the connfinder library.
// This class is pretty stupid and is managed by the ConnVector class. It
// really just serves to hold data.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CONNFINDER_CONNECTION_H
#define API_CONNFINDER_CONNECTION_H

#include <string>
#include <opencv/cv.h>

#include "Point.h"
#include "PeakVector.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class Connection
// -----------------------------------------------------------------------------

class Connection
{
private:
    // The index (id) of the first peak and the second peak. Index numbers
    // should be managed by the caller.
    unsigned int m_peak1;
    unsigned int m_peak2;

    // The 2 points that form the connection.
    Point m_point1;
    Point m_point2;
    
public:
    // Constructor.
    Connection(
            unsigned int peak1, unsigned int peak2,
            Point point1, Point point2) :
        m_peak1(peak1),
        m_peak2(peak2),
        m_point1(point1),
        m_point2(point2)
    {
        // no-op
    }

    // Accessors
    unsigned int GetPeak1() const {return m_peak1;}
    unsigned int GetPeak2() const {return m_peak2;}

    const Point& GetPoint1() const {return m_point1;}
    const Point& GetPoint2() const {return m_point2;}

    unsigned int GetTick1(const PeakVector& lst) const;
    unsigned int GetTick2(const PeakVector& lst) const;

    std::string ToString() const;
};

} // end namespace cs

#endif
