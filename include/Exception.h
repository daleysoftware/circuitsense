// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_EXCEPTION_EXCEPTION_H
#define API_EXCEPTION_EXCEPTION_H

#include <string>

namespace cs
{

// -----------------------------------------------------------------------------
// class Exception
// -----------------------------------------------------------------------------

class Exception
{
private:
    // The error message.
    std::string m_message;

public:
    // Constructor
    Exception(const std::string& message) : m_message(message)
    {
        // no-op
    }

    // Get the error message.
    const std::string& what() const;
};

} // end namespace cs

#endif
