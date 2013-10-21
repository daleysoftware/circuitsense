// -----------------------------------------------------------------------------
// The Circuit class represents a real electrical circuit. Unlike the old
// implementation, however, we distinguish between circuit "entities" and
// circuit "components".
//
// A circuit entity can be either a circuit component, a circuit node (as in a
// wire), or text (as in nothing). These notions are relected in the remove
// operation because when an entity is removed, the connections routed through
// that node are preserved.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CIRCUIT_CIRCUIT_H
#define API_CIRCUIT_CIRCUIT_H

#include <vector>
#include <string>

#include "Tesseract.h"
#include "CircuitEntity.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class Circuit
// -----------------------------------------------------------------------------

// Please note that this class will throw an exception whenever the user
// provides an invalid index to a public method.
class Circuit
{
private:
    // Our vector of circuit entities.
    typedef std::vector<CircuitEntity> EntityVector;
    EntityVector m_entities;

    // The current (next) maximum node number. Reserve 0 for ground nodes. The
    // netlist generator deals with ground node handling.
    unsigned int m_maxnode;

    // -------------------------------------------------------------------------
    // Private Mutators
    // -------------------------------------------------------------------------

    // Sorts the list using the sort predicate defined by the component ID
    // manager. Not a public function because it will change all the index
    // numbers associated with the components that have already been added.
    void Sort();

    // Add m_maxnode to all nodes, then go through the list of entities and do a
    // replace node sequentially. The result is a circuit with a sorted,
    // predictable node pattern.
    //
    // Does not touch ground nodes.
    void SortNodes();

    // Remove the entity at the given index. This function is private because it
    // will change all the index numbers for the entities currently in our
    // entity list.
    void RemoveEntity(unsigned int index);

    // Remove all connection references to the given node. Used by the above
    // RemoveEntity function.
    void RemoveNode(unsigned int node);

    // Shift the node numbers down. Updates the m_maxnode member if appopriate.
    //
    // Note that this is a very expensive operation and it might make this
    // implementation seem silly. This is being done so that it is easy to
    // create a netlist for a given circuit. This, after all, is the end game.
    void FixNodeNumbers();

    // Replace obsoleteNode with replaceWith in all of our entities.
    void ReplaceNodeNumber(unsigned int obsoleteNode, unsigned int replaceWith);

public:
    // Constructor. Reserve node 0 for grounds.
    Circuit() : m_maxnode(1)
    {
        // no-op
    }

    // -------------------------------------------------------------------------
    // Accessors
    // -------------------------------------------------------------------------

    // Get the number of entities currently in this circuit object.
    unsigned int GetNumEntities() const;

    // Get the number of entities currently in this circuit object that have one
    // or more connections.
    unsigned int GetNumConnectedEntities() const;

    // Long name pretty much sums it up.
    unsigned int GetNumConnectedNotNullEntities() const;
    unsigned int GetNumDisconnectedNullEntities() const;
    unsigned int GetNumDisconnectedNullEntitiesWithValue() const;

    // Get the number of connections for the entity at the given index.
    unsigned int GetNumConnections(unsigned int index) const;

    // Get the number of sequential connections for the entity at the given
    // index.
    //
    // Define sequential connections with the following example: we have 3
    // connections in total on ports 0, 1, and 3. We do not have a connection
    // registered on port 2. In this case, we have 3 connections and 2
    // sequential connections.
    unsigned int GetNumSequentialConnections(unsigned int index) const;

    // Return the component ID for the entity at the given index.
    SmartComponentID GetComponentID(unsigned int index) const;

    // Get the value of the entity at index.
    const std::string& GetValue(unsigned int index) const;

    // Return true if the entity at index has a value.
    bool HasValue(unsigned int index) const;

    // Returns true when the number of connections for the entity at the given
    // index is not zero.
    bool IsConnected(unsigned int index) const;

    // Returns true if the entities corresponding to the given index values are
    // connected at the given ports.
    bool IsConnected(
            unsigned int index1, unsigned int port1,
            unsigned int index2, unsigned int port2) const;

    std::string ToString() const;

    // -------------------------------------------------------------------------
    // Mutators
    // -------------------------------------------------------------------------

    // Add an entity with the given circuit component ID. Return the index of
    // the component that we just added.
    //
    // Uses a default value string.
    unsigned int AddEntity(SmartComponentID compid);
    unsigned int AddEntity(SmartComponentID compid, std::string value);

    // Set the component ID for the entity at the given index.
    void SetComponentID(unsigned int index, SmartComponentID compid);

    // Set the value of the entity at index.
    void SetValue(unsigned int index, std::string value);

    // Add a connection between entities with indices index1 and index2 at the
    // given ports port1 and port2.
    void AddConnection(
            unsigned int index1, unsigned int port1,
            unsigned int index2, unsigned int port2);

    // -------------------------------------------------------------------------
    // Friends
    // -------------------------------------------------------------------------

    // Friends so the netlist generator has access to the private remove
    // functions.
    friend class NetlistGenerator;
};

} // end namespace cs

#endif
