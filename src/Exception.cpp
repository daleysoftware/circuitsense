// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include "Exception.h"

namespace cs
{

const std::string& Exception::what() const
{
    return m_message;
}

} // end namespace cs
