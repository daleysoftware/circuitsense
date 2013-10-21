// -----------------------------------------------------------------------------
// This class is also pretty stupid and the node numbers are managed by the
// circuit class. That is why we expose the connection pointers to the caller.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CIRCUIT_CIRCUITENTITY_H
#define API_CIRCUIT_CIRCUITENTITY_H

#include <string>
#include <vector>

#include "CircuitEntityConnection.h"
#include "SmartComponentID.h"
#include "Tesseract.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class CircuitEntity
// -----------------------------------------------------------------------------

class CircuitEntity
{
public:
    // Our vector of connections.
    typedef std::vector<CircuitEntityConnection> ConnectionVector;
    ConnectionVector m_vector;

private:
    // This entity's component ID.
    SmartComponentID m_compid;

    // This entity's value string.
    std::string m_value;

public:
    // Constructor.
    CircuitEntity(SmartComponentID compid, std::string value) :
        m_compid(compid),
        m_value(value)
    {
        // no-op
    }

    // Accessors
    unsigned int GetNumConnections() const;
    unsigned int GetNumSequentialConnections() const;
    bool IsConnected() const;
    SmartComponentID GetComponentID() const;
    const std::string& GetValue() const;
    bool HasValue() const;
    unsigned int GetNumConnectionsWithNode(unsigned int node) const;
    int GetMinNode() const;

    std::string ToString() const;

    // Mutators
    void SetComponentID(SmartComponentID compid);
    void SetValue(std::string value);

    CircuitEntityConnection RemoveOne();
    void RemoveByNode(unsigned int node);

    // Add the given connection to the connection vector. First checks to see if a connection
    // exists on the given port. If it does exist, then that old connection is replaced with this
    // one (i.e. the old node number is overwritten).
    //
    // The caller is responsible for managing the implications of overwriting node numbers.
    //
    // Returns -1 if there was no connection on the given port. Return the old node number that we
    // just wrote over if there was such a connection.
    int AddConnection(CircuitEntityConnection conn);

    // If a zero pointer is returned, no such connection entity exists. Do not free the returned
    // pointer! It belongs to this class.
    CircuitEntityConnection* GetConnectionByPort(unsigned int port);
    const CircuitEntityConnection* GetConstConnectionByPort(unsigned int port) const;
    
    CircuitEntityConnection* GetConnectionByNode(unsigned int node);
    const CircuitEntityConnection* GetConstConnectionByNode(unsigned int node) const;

    // Direct access to the vector.
    ConnectionVector& GetVector() {return m_vector;}
};

}

#endif
