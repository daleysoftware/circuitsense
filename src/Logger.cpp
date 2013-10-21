// -----------------------------------------------------------------------------
// This header contains the implmenetation for the general purpose logging class
// as part of the debug library.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <string>
#include <sys/time.h>

#include "Logger.h"

namespace cs
{

std::string Logger::GetTimeStamp()
{
    time_t t;
    time(&t);

    char buff[9];
    strftime(buff, 9, "%H:%M:%S", localtime(&t));

    return std::string(buff);
}

void Logger::Log(const std::string& s)
{
    std::cout << GetTimeStamp() << " " << s << std::endl;
}

void operator<< (Logger& out, const std::string& s)
{
    out.Log(s);
}

} // end namespace cs
