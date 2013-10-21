// -----------------------------------------------------------------------------
// Author: Matt Pillar
//
// This file contains the implementation of classes used for analyzing execution
// time in the IP process algorithm.
//
// Copyright (c) 2011 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <vector>

#include "TimeAnalyzer.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class TimeAnalyzerNode
// -----------------------------------------------------------------------------

void TimeAnalyzerNode::Print()
{
    uint64_t diff = m_end - m_start;
    double diffSeconds = diff / 1000.0;

    for (unsigned int i = 0; i < m_depth; i++)
    {
        std::cout << "\t";
    }

    std::cout << diffSeconds << " sec(s): " << m_identifier << std::endl;
}

// -----------------------------------------------------------------------------
// class TimeAnalyzer
// -----------------------------------------------------------------------------

static TimeAnalyzer* g_instance = 0;

TimeAnalyzer& TimeAnalyzer::Instance()
{
    if (g_instance == 0)
    {
        g_instance = new TimeAnalyzer();
    }

    return *g_instance;
}

void TimeAnalyzer::PrintAnalysis()
{
    if (m_depth > 0)
    {
        std::cout << "WARNING: non-zero depth in print analysis" << std::endl;
    }

    std::deque<TimeAnalyzerNode>::iterator it;
    for (it = m_done.begin(); it != m_done.end(); it++)
    {
        it->Print();
    }

    uint64_t total = m_done.back().m_end - m_done.front().m_start;
    double totalSecs = total / 1000.0;

    printf("Total time: %f sec(s)\n", totalSecs);
}

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

void PushMessage(std::string message)
{
    TimeAnalyzer::Instance().Push(message);
}

void PopMessage()
{
    TimeAnalyzer::Instance().Pop();
}

void PrintAnalysis()
{
    TimeAnalyzer::Instance().PrintAnalysis();
}

} // end namespace cs
