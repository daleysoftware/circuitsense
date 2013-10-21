// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_EXCEPTION_TERMINATEDEXCEPTION_H
#define API_EXCEPTION_TERMINATEDEXCEPTION_H

#include <string>
#include "Exception.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class TerminatedException
// -----------------------------------------------------------------------------

class TerminatedException : public Exception
{
public:
    // Constructor
    TerminatedException(const std::string& message) :
        Exception(message)
    {
        // Nothing to do.
    }
};

} // end namespace cs

#endif
