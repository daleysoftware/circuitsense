// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <string>
#include <sys/time.h>

#include "GetTime.h"

namespace cs
{

uint64_t GetTime::Milli()
{
    struct timeval current;
    gettimeofday(&current, NULL);

    // Make sure to cast as 64 bit unsigned before doing math, otherwise
    // implicit int will roll over.
    return
        ((uint64_t) current.tv_sec) * 1000 +
        ((uint64_t) current.tv_usec) / 1000;
}

time_t GetTime::Secs()
{
    time_t t;
    time(&t);
    return t;
}

std::string GetTime::String()
{
    time_t t;
    time(&t);

    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));

    return std::string(buff);
}

std::string GetTime::ShortString()
{
    time_t t;
    time(&t);

    char buff[9];
    strftime(buff, 9, "%H:%M:%S", localtime(&t));

    return std::string(buff);
}

} // namespace cs
