// -----------------------------------------------------------------------------
// This class is pretty stupid, and is managed by the connection whiteout
// object.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CONNFINDER_CONNVECTOR_H
#define API_CONNFINDER_CONNVECTOR_H

#include <vector>
#include <opencv/cv.h>

#include "PeakFinder.h"
#include "Connection.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class ConnVector
// -----------------------------------------------------------------------------

class ConnVector
{
public:
    typedef std::vector<Connection> ConnectionVector;

private:
    // Our list of  connections.
    ConnectionVector m_conns;

public:
    // Constructor.
    ConnVector()
    {
        // no-op
    }

    // Mutators
    void Push(Connection c);

    // Accessors
    std::string ToString() const;
    const ConnectionVector& GetVector() const {return m_conns;}
};

} // end namespace cs

#endif
