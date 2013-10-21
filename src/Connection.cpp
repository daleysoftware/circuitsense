// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <math.h>
#include <iostream>

#include "Connection.h"

namespace cs
{

std::string Connection::ToString() const
{
    std::ostringstream os;

    os << "{"
        << m_peak1 << ":" << m_point1.ToString()
        << m_peak2 << ":" << m_point2.ToString()
        << "}";

    return os.str();
}

} // end namespace cs
