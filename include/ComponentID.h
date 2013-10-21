// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TESS_COMPONENTID_H
#define API_TESS_COMPONENTID_H

#include <string>

namespace cs
{

// -----------------------------------------------------------------------------
// class ComponentID
// -----------------------------------------------------------------------------

class ComponentID
{
private:
    // Our id.
    char m_id;

public:
    // Constructor. Intentionally do not use an explicit constructor.
    ComponentID(char id) : m_id(id)
    {
        // no-op
    }

    // Accessors
    char GetID() const {return m_id;}
    std::string ToString() const;

    // Mutators
    void SetID(char id) {m_id = id;}
};

}

#endif
