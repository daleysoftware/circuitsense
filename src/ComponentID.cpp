// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <string>
#include <sstream>

#include "ComponentID.h"

namespace cs
{

std::string ComponentID::ToString() const
{
    std::ostringstream os;
    os << GetID();
    return os.str();
}

} // end namespace cs
