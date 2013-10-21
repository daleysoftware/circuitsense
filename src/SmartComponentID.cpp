// -----------------------------------------------------------------------------
// Implementation of the circuit library. This particular class is used to
// map component ID's to semantic types of circuit components. This class also
// contains some sorting related functionality.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <cstring>
#include <climits>

#include "Circuit.h"

namespace cs
{

// Utility for the sorting predicate. New components must be added somewhere to
// this sorting predicate.
//
// When 'sort' is true, the unsigned returned corresponds to the value that
// should be used by the sorting predicate. When false, the unsigned returned
// corresponds to the value that should be used by the operator== overload.
static unsigned int ComponentIDToUnsigned(
        bool sort,
        const SmartComponentID& comp)
{
    // Note the components should be grouped by equality in this switch
    // statement.
    switch(comp.GetID())
    {
        // Active components.
        case 'S': return 0;
        case 's': return sort ? 1 : 0;
        case 'V': return sort ? 2 : 2;
        case 'v': return sort ? 3 : 2;
        case 'A': return sort ? 4 : 2;
        case 'a': return sort ? 5 : 2;

        // Passive components
        case 'R': return 100;
        case 'r': return sort ? 101 : 100;

        case 'C': return 102;
        case 'c': return sort ? 103 : 102;

        case 'L': return 104;
        case 'l': return sort ? 105 : 104;

        case 'D': return 106;
        case 'd': return sort ? 107 : 106;
        case 'E': return sort ? 108 : 106;
        case 'e': return sort ? 109 : 106;

        case 'N': return 110;
        case 'n': return sort ? 111 : 110;

        case 'P': return 112;
        case 'p': return sort ? 113 : 112;

        case 'T': return 114;
        case 't': return sort ? 115 : 114;

        case 'Q': return 116;
        case 'q': return sort ? 117 : 116;

        // Put unknowns at the bottom of the list.
        default: return INT_MAX;
    }
}

bool SmartComponentID::operator==(const SmartComponentID& id)
{
    return ComponentIDToUnsigned(false, *this) ==
           ComponentIDToUnsigned(false, id);
}

bool ComponentIDSortPredicate(
        const CircuitEntity& entity1,
        const CircuitEntity& entity2)
{
    return ComponentIDToUnsigned(true, entity1.GetComponentID()) <
           ComponentIDToUnsigned(true, entity2.GetComponentID());
}

bool SmartComponentID::IsGround() const
{
    return GetID() == 'G';
}

bool SmartComponentID::IsNull() const
{
    return GetID() == '\0';
}

}
