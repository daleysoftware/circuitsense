// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <sstream>
#include <iostream>
#include <stdio.h>

#include "Circuit.h"
#include "Exception.h"

namespace cs
{

unsigned int CircuitEntity::GetNumConnections() const
{
    return m_vector.size();
}

unsigned int CircuitEntity::GetNumSequentialConnections() const
{
    unsigned int counter = 0;

    // We assume we are sorted. The add connection class will always sort us by
    // port when a new connection is added, making this assumption valid.
    for (ConnectionVector::const_iterator it = m_vector.begin();
            it != m_vector.end(); it++, counter++)
    {
        if (it->GetPort() != counter)
        {
            break;
        }
    }

    return counter;
}

bool CircuitEntity::IsConnected() const
{
    return m_vector.size() > 0;
}

SmartComponentID CircuitEntity::GetComponentID() const
{
    return m_compid;
}

const std::string& CircuitEntity::GetValue() const
{
    return m_value;
}

bool CircuitEntity::HasValue() const
{
    return !m_value.empty();
}

unsigned int CircuitEntity::GetNumConnectionsWithNode(unsigned int node)
    const
{
    unsigned int result = 0;
    for (ConnectionVector::const_iterator it = m_vector.begin();
            it != m_vector.end(); it++)
    {
        if (it->GetNode() == node)
        {
            result++;
        }
    }

    return result;
}

int CircuitEntity::GetMinNode() const
{
    int result = -1;

    for (ConnectionVector::const_iterator it = m_vector.begin();
            it != m_vector.end(); it++)
    {
        if (result == -1 || it->GetNode() < result)
        {
            result = it->GetNode();
        }
    }

    return result;
}

std::string CircuitEntity::ToString() const
{
    std::ostringstream os;
    os
        << "compid='" << GetComponentID().ToString() << "'"
        << " value='" << GetValue() << "'";

    if (m_vector.empty())
    {
        return os.str();
    }

    os << std::endl;
    for (ConnectionVector::const_iterator it = m_vector.begin();
            it != m_vector.end(); it++)
    {
        os << "\t" << it->ToString() << std::endl;
    }

    return os.str().substr(0, os.str().size()-1);
}

void CircuitEntity::SetComponentID(SmartComponentID compid)
{
    m_compid = compid;
}

void CircuitEntity::SetValue(std::string value)
{
    m_value = value;
}

CircuitEntityConnection CircuitEntity::RemoveOne()
{
    if (GetNumConnections() == 0)
    {
        throw Exception("Cannot RemoveOne when the conn vector is empty");
    }

    CircuitEntityConnection result(0,0);
    result = m_vector.back();
    m_vector.pop_back();

    return result;
}

void CircuitEntity::RemoveByNode(unsigned int node)
{
    bool more;
    do
    {
        more = false;
        for (ConnectionVector::iterator it = m_vector.begin();
                it != m_vector.end(); it++)
        {
            if (it->GetNode() == node)
            {
                // Remove this connection.
                m_vector.erase(it);

                // Break out of the loop and start over, because the call to
                // erase has invalidated our iterator.
                more = true;
                break;
            }
        }
    }
    while (more);
}

static bool PortSortPredicate(
        const CircuitEntityConnection& c1,
        const CircuitEntityConnection& c2)
{
    return c1.GetPort() < c2.GetPort();
}

int CircuitEntity::AddConnection(CircuitEntityConnection conn)
{
    // First, search to see if there exists a connection at the given port. If
    // there is, replace that connection node with the new one and return the 
    // old obsolete node for the caller to handle.
    for (ConnectionVector::iterator it = m_vector.begin(); it != m_vector.end();
            it++)
    {
        if (it->GetPort() == conn.GetPort())
        {
            unsigned int node = it->GetNode();
            it->SetNode(conn.GetNode());
            return node;
        }
    }

    // No connection with the given port was found, so just push this on onto
    // the vector.
    m_vector.push_back(conn);

    // Since we just added a connection, sort the vector.
    std::sort(m_vector.begin(), m_vector.end(), PortSortPredicate);

    return -1;
}

// Search predicate for port. Use a static global for data access.
static int g_port = 0;
static bool PortFindPredicate(const CircuitEntityConnection& conn)
{
    return conn.GetPort() == g_port;
}

CircuitEntityConnection* CircuitEntity::GetConnectionByPort(
        unsigned int port)
{
    g_port = port;
    ConnectionVector::iterator it = std::find_if(m_vector.begin(),
            m_vector.end(), PortFindPredicate);

    return it == m_vector.end() ? 0 : &(*it);
}

const CircuitEntityConnection*
        CircuitEntity::GetConstConnectionByPort(
        unsigned int port) const
{
    g_port = port;
    ConnectionVector::const_iterator it = std::find_if(m_vector.begin(),
            m_vector.end(), PortFindPredicate);

    return it == m_vector.end() ? 0 : &(*it);
}

// Search predicate for node. Use a static global for data access.
static int g_node = 0;
static bool NodeFindPredicate(const CircuitEntityConnection& conn)
{
    return conn.GetNode() == g_node;
}

CircuitEntityConnection* CircuitEntity::GetConnectionByNode(
        unsigned int node)
{
    g_node = node;
    ConnectionVector::iterator it = std::find_if(m_vector.begin(),
            m_vector.end(), NodeFindPredicate);

    return it == m_vector.end() ? 0 : &(*it);
}

const CircuitEntityConnection*
        CircuitEntity::GetConstConnectionByNode(
        unsigned int node) const
{
    g_node = node;
    ConnectionVector::const_iterator it = std::find_if(m_vector.begin(),
            m_vector.end(), NodeFindPredicate);

    return it == m_vector.end() ? 0 : &(*it);
}

} // end namespace cs
