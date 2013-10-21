// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <string>
#include <iostream>
#include <sstream>

#include "DebugRow.h"

namespace cs
{

std::string DebugRow::ToString() const
{
    std::ostringstream os;
    os
        << m_threshold << ","
        << m_confidence << ","
        << m_components << ","
        << m_textConfidence << ","
        << m_textComponents;

    return os.str();
}

} // end namespace cs
