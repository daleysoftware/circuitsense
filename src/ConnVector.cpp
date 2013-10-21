// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>

#include "ConnFinder.h"

namespace cs
{

void ConnVector::Push(Connection c)
{
    m_conns.push_back(c);
}

std::string ConnVector::ToString() const
{
    if (m_conns.size() == 0)
    {
        return "{}";
    }

    std::ostringstream os;

    for (ConnectionVector::const_iterator it = m_conns.begin();
            it != m_conns.end(); it++)
    {
        os << it->ToString() << std::endl;
    }

    return os.str().substr(0, os.str().size()-1);
}

} // end namespace cs
