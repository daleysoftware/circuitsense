// -----------------------------------------------------------------------------
// Implementation of the circuit library. This particular class holds
// information related to a circuit object and converts it to a netlist.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>

#include "Circuit.h"

namespace cs
{

// -----------------------------------------------------------------------------
// Accessors
// -----------------------------------------------------------------------------

unsigned int Circuit::GetNumEntities() const
{
    return m_entities.size();
}

unsigned int Circuit::GetNumConnectedEntities() const
{
    unsigned int connectedEntities = 0;
    for (EntityVector::const_iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        if (it->IsConnected())
        {
            connectedEntities++;
        }
    }

    return connectedEntities;
}

unsigned int Circuit::GetNumConnectedNotNullEntities() const
{
    unsigned int entities = 0;
    for (EntityVector::const_iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        if (it->IsConnected() && !it->GetComponentID().IsNull())
        {
            entities++;
        }
    }

    return entities;
}

unsigned int Circuit::GetNumDisconnectedNullEntities() const
{
    unsigned int entities = 0;
    for (EntityVector::const_iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        if (!it->IsConnected() &&
            it->GetComponentID().IsNull())
        {
            entities++;
        }
    }

    return entities;
}

unsigned int Circuit::GetNumDisconnectedNullEntitiesWithValue() const
{
    unsigned int entities = 0;
    for (EntityVector::const_iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        if (!it->IsConnected() &&
            it->GetComponentID().IsNull() &&
            it->HasValue())
        {
            entities++;
        }
    }

    return entities;
}

unsigned int Circuit::GetNumConnections(unsigned int index) const
{
    return m_entities[index].GetNumConnections();
}

unsigned int Circuit::GetNumSequentialConnections(unsigned int index) const
{
    return m_entities[index].GetNumSequentialConnections();
}

SmartComponentID Circuit::GetComponentID(unsigned int index) const
{
    return m_entities[index].GetComponentID();
}

const std::string& Circuit::GetValue(unsigned int index) const
{
    return m_entities[index].GetValue();
}

bool Circuit::HasValue(unsigned int index) const
{
    return m_entities[index].HasValue();
}

bool Circuit::IsConnected(unsigned int index) const
{
    return m_entities[index].IsConnected();
}

bool Circuit::IsConnected(
        unsigned int index1, unsigned int port1,
        unsigned int index2, unsigned int port2) const
{
    const CircuitEntityConnection* entity1 =
        m_entities[index1].GetConstConnectionByPort(port1);
    const CircuitEntityConnection* entity2 =
        m_entities[index2].GetConstConnectionByPort(port2);

    if (entity1 == 0 || entity2 == 0)
    {
        return false;
    }

    return entity1->GetNode() == entity2->GetNode();
}

std::string Circuit::ToString() const
{
    if (GetNumEntities() == 0)
    {
        return std::string();
    }

    std::ostringstream os;
    unsigned int index = 0;

    for (EntityVector::const_iterator it = m_entities.begin();
            it != m_entities.end(); it++, index++)
    {
        os << "index=" << index << " " << it->ToString() << std::endl;
    }

    return os.str().substr(0, os.str().size()-1);
}

// -----------------------------------------------------------------------------
// Mutators
// -----------------------------------------------------------------------------

unsigned int Circuit::AddEntity(SmartComponentID compid)
{
    return AddEntity(compid, std::string());
}

unsigned int Circuit::AddEntity(SmartComponentID compid,
        std::string value)
{
    m_entities.push_back(CircuitEntity(compid, value));
    return m_entities.size()-1;
}

void Circuit::SetComponentID(unsigned int index,
        SmartComponentID compid)
{
    m_entities[index].SetComponentID(compid);
}

void Circuit::SetValue(unsigned int index, std::string value)
{
    m_entities[index].SetValue(value);
}

void Circuit::AddConnection(
        unsigned int index1, unsigned int port1,
        unsigned int index2, unsigned int port2)
{
    // Note there is no special handling for ground because we might change the
    // component ID anyway. The ground handling is done in the netlist
    // generator.

    // Add the connections and use the return value to see if we need to fix up
    // our node numbers.
    int result1 = m_entities[index1].AddConnection(
            CircuitEntityConnection(port1, m_maxnode));
    int result2 = m_entities[index2].AddConnection(
            CircuitEntityConnection(port2, m_maxnode));

    bool shift = false;
    if (result1 >= 0)
    {
        ReplaceNodeNumber(result1, m_maxnode);
        shift = true;
    }
    if (result2 >= 0)
    {
        ReplaceNodeNumber(result2, m_maxnode);
        shift = true;
    }

    // Increment the next max node.
    m_maxnode++;

    if (shift)
    {
        // Shift the node numbers down.
        FixNodeNumbers();
    }
}

// -----------------------------------------------------------------------------
// Private Mutators
// -----------------------------------------------------------------------------

// This is actually defined in SmartComponentID. This was done to minimize
// the number of places code needs to be changed when a new component is added.
bool ComponentIDSortPredicate(
        const CircuitEntity& entity1,
        const CircuitEntity& entity2);

void Circuit::Sort()
{
    std::sort(m_entities.begin(), m_entities.end(),
            ComponentIDSortPredicate);
}

void Circuit::SortNodes()
{
    // Add m_maxnode to every node in every entity.
    for (EntityVector::iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        for (CircuitEntity::ConnectionVector::iterator cit =
                it->GetVector().begin();
                cit != it->GetVector().end(); cit++)
        {
            if (cit->GetNode() != 0)
            {
                cit->SetNode(cit->GetNode() + m_maxnode);
            }
        }
    }

    // Now go through and replace nodes sequentially. Assuming the entities are
    // sorted, the result is a nice predictable netlist.
    unsigned int counter = 1;
    for (EntityVector::iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        for (CircuitEntity::ConnectionVector::iterator cit =
                it->GetVector().begin();
                cit != it->GetVector().end(); cit++)
        {
            if (cit->GetNode() > m_maxnode)
            {
                ReplaceNodeNumber(cit->GetNode(), counter);
                counter++;
            }
        }
    }
}

void Circuit::RemoveEntity(unsigned int index)
{
    if (m_entities[index].GetNumConnections() > 0)
    {
        // The node that we will change the others to. This must be the lowest
        // node attached to this entity.
        unsigned int changeNode = m_entities[index].GetMinNode();

        // While this node still has some connections...
        while (m_entities[index].GetNumConnections() > 0)
        {
            // Replace the node with the change node.
            CircuitEntityConnection conn = m_entities[index].RemoveOne();
            ReplaceNodeNumber(conn.GetNode(), changeNode);

        }

        // Check if we need to remove the node corresponding to this connection
        // (the change node). We'll have to seach for it within all entities.
        unsigned int connections = 0;

        for (EntityVector::const_iterator it = m_entities.begin();
                it != m_entities.end(); it++)
        {
            connections += it->GetNumConnectionsWithNode(changeNode);

            if (connections > 1)
            {
                // No need to search anymore.
                break;
            }
        }

        if (connections < 2)
        {
            // Two cases:
            //  1. Only one port connected to this node (we have severed the
            //     connection).
            //  2. There is nothing connected to this node anymore.
            //
            // In either case, we need to remove references to this node.
            RemoveNode(changeNode);
        }
    }

    // The node's connections have been deleted and so have the references to
    // stray nodes.
    //
    // Now, delete the entity at index and fix the node numbers, in case we
    // broken them.

    m_entities.erase(m_entities.begin()+index);
    FixNodeNumbers();
}

void Circuit::RemoveNode(unsigned int node)
{
    for (EntityVector::iterator it = m_entities.begin();
            it != m_entities.end(); it++)
    {
        it->RemoveByNode(node);
    }

    // Fix the node numbers because we have deleted a node. It might be the case
    // that there actually was no node deleted, but attempt to fix anyway.
    //
    // Since this function is only called internall, we assume there was a
    // reason for wanting to delete the node.
    FixNodeNumbers();
}

void Circuit::FixNodeNumbers()
{
    // Loop through all the node numbers that we currently think we have. Do not
    // start at the zero node, those are reserved for grounds.
    for (unsigned int node = 1; node < m_maxnode; node++)
    {
        bool found = false;

        // Now loop through all the entities and see if we can find this node
        // number.
        for (EntityVector::const_iterator it = m_entities.begin();
                it != m_entities.end(); it++)
        {
            const CircuitEntityConnection* connection =
                it->GetConstConnectionByNode(node);

            if (connection != 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            // There are no references to this node. Replaces all instances of
            // m_maxnode-1 with this node, and decrement the max node.
            ReplaceNodeNumber(m_maxnode-1, node);
            m_maxnode--;
        }
    }
}

void Circuit::ReplaceNodeNumber(unsigned int obsoleteNode,
        unsigned int replaceWith)
{
    // First, iterator through our entitiies.
    for (EntityVector::iterator it = m_entities.begin(); it != m_entities.end();
            it++)
    {
        // See if this entity has a connection on the obsolete node.
        CircuitEntityConnection* entity =
            it->GetConnectionByNode(obsoleteNode);

        if (entity != 0)
        {
            // They have a connection with this node. Update it to use the new
            // node.
            entity->SetNode(replaceWith);
        }
    }
}

} // end namespace cs
