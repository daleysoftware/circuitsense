// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <string>
#include <sstream>

#include "ResultCircuits.h"
#include "Tesseract.h"

namespace cs
{

std::string ResultCircuits::VectorToString(const CircuitVector& vec)
{
    if (vec.empty())
    {
        return std::string();
    }

    std::ostringstream os;

    for (CircuitVector::const_iterator it = vec.begin(); it != vec.end(); it++)
    {
        os << it->ToString() << ",";
    }

    return os.str().substr(0, os.str().size()-1);
}

} // end namespace cs
