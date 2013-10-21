// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "CircuitEntityConnection.h"

namespace cs
{

std::string CircuitEntityConnection::ToString() const
{
    std::ostringstream os;
    os << "port=" << GetPort() << " -> node=" << GetNode();
    return os.str();
}

} // end namespace cs
