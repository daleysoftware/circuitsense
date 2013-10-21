// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CONNFINDER_WHITEOUTCONNFINDER_H
#define API_CONNFINDER_WHITEOUTCONNFINDER_H

#include <stdint.h>
#include <opencv/cv.h>

#include "Rectangle.h"
#include "Point.h"
#include "Whiteout.h"
#include "WhiteoutPerimeter.h"
#include "ConnVector.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class WhiteoutConnFinder
// -----------------------------------------------------------------------------

class WhiteoutConnFinder : public Whiteout
{
private:
    //  connection vector.
    ConnVector m_conns;

    // Reference to the peak vector.
    const PeakVector& m_peaks;

    // The poitn param to the grayout function.
    Point m_graypoint;

    // The index of the rectangle the grayout function is currently working on.
    unsigned int m_index;

    // Perimeter whiteout object.
    WhiteoutPerimeter m_pwhite;

protected:
    // The addditional check that we're not within the rectangle.
    virtual bool AdditionalCheck(const Point& p);

    // Point found: have we found a peak? If yes, add the peak to the peak conn
    // matrix, and colour out the border.
    virtual void PointFound(const Point& p);

public:
    // Constructor.
    WhiteoutConnFinder(IplImage& src, const PeakVector& peaks) :
        Whiteout(src),
        m_conns(),
        m_peaks(peaks),
        m_graypoint(0, 0),
        m_index(0),
        m_pwhite(src)
    {
        // no-op
    }

    // Accessor
    const ConnVector& GetConnections() {return m_conns;}

    // Whiteout Derivative. Main function to find connections between two peaks.
    // Only mean to be used by the connection finder class.
    //
    // Return true if a peak was found while searching off this point.
    bool GreyoutConnection(const Point& p);
};

} // end namespace cs

#endif
