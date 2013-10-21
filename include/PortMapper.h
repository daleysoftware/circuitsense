// -----------------------------------------------------------------------------
// This library converts the given connection and peak vectors to a circuit
// object.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_PORTMAPPER_PORTMAPPER_H
#define API_PORTMAPPER_PORTMAPPER_H

#include <opencv/cv.h>

#include "ConnFinder.h"
#include "Circuit.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class PortMapper
// -----------------------------------------------------------------------------

class PortMapper
{
private:
    // The input peaks and connections.
    const PeakVector& m_peaks;
    const ConnVector& m_conns;

public:
    // Constructor.
    PortMapper(
            const PeakVector& peaks,
            const ConnVector& conns) :
        m_peaks(peaks),
        m_conns(conns)
    {
        // no-op
    }

    // Convert the given peak vector and connection vector to a circuit object.
    // This requires the calculation of the port numbers based on the 3 o'clock
    // port numbering convention.
    Circuit ConvertToCircuit();
};

} // end namespace cs

#endif
