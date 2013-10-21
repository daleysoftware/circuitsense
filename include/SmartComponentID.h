// -----------------------------------------------------------------------------
// The smart ComponentID class is used to map circuit component ID's to semantic
// circuit meanings, i.e. functionality like IsGround() or IsResistor().
//
// Another spot for this code might've been in the tesseract library, but
// since that library really doesn't care about what type of component it is,
// I put it here.
//
// This implementation file, and the netlist generator implementation file, are
// the only files you need to change if you add a new circuit component to the
// tesseract training library.
//
// Copyright (c) 2011-2012Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CIRCUIT_SMARTCOMPONENTID_H
#define API_CIRCUIT_SMARTCOMPONENTID_H

#include "ComponentID.h"
#include "Tesseract.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class SmartComponentID
// -----------------------------------------------------------------------------

class SmartComponentID : public ComponentID
{
public:
    // Intentially do not use an explicit constructor.
    //
    // If we ever move away from characters, then the parent class will update
    // their constructor and we will in turn have to update ours.
    SmartComponentID(char id) : ComponentID(id)
    {
        // no-op
    }

    // Operator overloads.
    bool operator==(const SmartComponentID& id);

    // Circuit related accessors
    bool IsNull() const;
    bool IsGround() const;
};

} // end namespace cs

#endif
