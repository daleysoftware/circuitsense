// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CIRCUIT_CIRCUITENTITYCONNECTION_H
#define API_CIRCUIT_CIRCUITENTITYCONNECTION_H

#include <string>

namespace cs
{

// -----------------------------------------------------------------------------
// class CircuitEntityConnection
// -----------------------------------------------------------------------------

class CircuitEntityConnection
{
private:
    unsigned int m_port;
    unsigned int m_node;

public:
    // Constructor.
    CircuitEntityConnection(unsigned int port, unsigned int node) :
        m_port(port),
        m_node(node)
    {
        // no-op
    }

    // Accessors
    unsigned int GetPort() const {return m_port;}
    unsigned int GetNode() const {return m_node;}

    std::string ToString() const;

    // Mutators
    void SetPort(unsigned int port) {m_port = port;}
    void SetNode(unsigned int node) {m_node = node;}
};

} // end namespace cs

#endif
