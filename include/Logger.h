// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_DEBUG_LOGGER_H
#define API_DEBUG_LOGGER_H

#include <iostream>
#include <sstream>

namespace cs
{

// -----------------------------------------------------------------------------
// class Logger
// -----------------------------------------------------------------------------

class Logger
{
private:
    std::string GetTimeStamp();

public:
    // Constructor.
    Logger()
    {
        // Nothing to do.
    }

    // Log a message.
    void Log(const std::string& s);
};

void operator<< (Logger& out, const std::string& s);

}

#endif
