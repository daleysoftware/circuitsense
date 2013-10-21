// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_THREADMASTER_H
#define API_THREADMASTER_H

#include <string>
#include <stdint.h>

namespace cs
{

// A collection of static functions that return time information.
class GetTime
{
public:
    // Return the current system time in milliseconds.
    static uint64_t Milli();

    // Return the current system time in seconds.
    static time_t Secs();

    // Return the current date in string format (%Y-%m-%d %H:M:%S).
    static std::string String();

    // Return the current time in string format (%H:M:%S).
    static std::string ShortString();
};

} // end namespace cs

#endif
