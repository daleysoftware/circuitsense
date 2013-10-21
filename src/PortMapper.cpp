// -----------------------------------------------------------------------------
// Implementation of the portmapper library. This library is used to
// translate a peakvector and a connection vector to a circuit object. The
// circuit object really represents the same information that is contained in
// the two input vectors, just in a more organized manner.
//
// The circuit object also can be created without requiring the two vectors
// using native Circuit mutators, which is convenient for unit testing and
// system testing.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <opencv/cv.h>

#include "PortMapper.h"
#include "OutsideRectangleIterator.h"

namespace cs
{

static unsigned int PointPeakToTick(
        const Rectangle& peak,
        const Point& point)
{
    // The number of 'ticks' until we reach the point we are searchng for. Each
    // tick is one increment as we iterate the rectangle starting at 3 o'clock.
    unsigned int ticks = 0;

    for (OutsideRectangleIterator rit(
                Space(
                    peak.GetX()+peak.GetWidth()+1,
                    peak.GetY()+peak.GetHeight()+1),
                peak,
                Point(peak.GetX()+peak.GetWidth(), peak.GetY()));
            rit.More(); rit++, ticks++)
    {
        // Count the ticks until we find the point.
        if (rit.GetCurrent() == point)
        {
            return ticks;
        }
    }

    // Should never happen unless the index we're passed is screwed up. Again,
    // this class is pretty dump and doesn't hold any of this data.
    throw Exception("Peak list does not contain the point we want");
    return 0;
}

// Little structure to hold two integers for tick counting.
struct TwoUnsignedInts
{
    unsigned int m_int1;
    unsigned int m_int2;
};

// Little static function used to loop through the tick vector and the
// connection vector.
//
// It is assumed the connection vector and the tick vector are the same length.
static unsigned int GetPortFromTicks(unsigned int ticks, unsigned int index,
        const std::vector<TwoUnsignedInts>& tickvec,
        const ConnVector& conns)
{
    assert(conns.GetVector().size() == tickvec.size());

    std::map<std::string, unsigned int> pointMap;
    unsigned int port = 0;

    for (unsigned int i = 0; i < tickvec.size(); i++)
    {
        if (conns.GetVector()[i].GetPeak1() == index &&
            tickvec[i].m_int1 < ticks)
        {
            // Only bump port if we haven't looked at this point already. The
            // point ToString() is unique. Better to use a hash lookup here
            // with O(ln(n)) than to go through the whole connection vector
            // again, which would be O(n).
            std::string point = conns.GetVector()[i].GetPoint1().ToString();

            if (pointMap.find(point) == pointMap.end())
            {
                pointMap[point] = 1;
                port++;
            }
        }

        if (conns.GetVector()[i].GetPeak2() == index &&
            tickvec[i].m_int2 < ticks)
        {
            std::string point = conns.GetVector()[i].GetPoint1().ToString();

            if (pointMap.find(point) == pointMap.end())
            {
                pointMap[point] = 1;
                port++;
            }
        }
    }

    return port;
}

Circuit PortMapper::ConvertToCircuit()
{
    std::vector<TwoUnsignedInts> tickvec;
    Circuit circuit;

    // Iterate through each of the connections.
    for (ConnVector::ConnectionVector::const_iterator it =
            m_conns.GetVector().begin();
            it != m_conns.GetVector().end(); it++)
    {
        unsigned int tick1 =
            PointPeakToTick(m_peaks.GetVector()[it->GetPeak1()],
                    it->GetPoint1());
        unsigned int tick2 =
            PointPeakToTick(m_peaks.GetVector()[it->GetPeak2()],
                    it->GetPoint2());

        TwoUnsignedInts ticks = {tick1, tick2};
        tickvec.push_back(ticks);
    }

    // Add the appropriate number of entities before we go ahead and start
    // adding connections.
    for (unsigned int i = 0; i < m_peaks.GetVector().size(); i++)
    {
        // Add null entities.
        circuit.AddEntity(0);
    }

    // We have calculated all the ticks and cached them in tickvec. Use the
    // GetPortFromTicks() utility above to find the ports and add the
    // connections to the circuit object.
    for (unsigned int i = 0; i < tickvec.size(); i++)
    {
        unsigned int index1 = m_conns.GetVector()[i].GetPeak1();
        unsigned int index2 = m_conns.GetVector()[i].GetPeak2();

        circuit.AddConnection(
                index1,
                GetPortFromTicks(tickvec[i].m_int1, index1, tickvec, m_conns),
                index2,
                GetPortFromTicks(tickvec[i].m_int2, index2, tickvec, m_conns));
    }

    return circuit;
}

} // end namespace cs
