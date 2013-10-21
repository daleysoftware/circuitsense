// -----------------------------------------------------------------------------
// This header file contains the declaration of classes used for analyzing how
// long certain sections of the main image processing algorithm are taking.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef ALGORITHM_TIMEANALYZER_H
#define ALGORITHM_TIMEANALYZER_H

#include <string>
#include <deque>
#include <GetTime.h>

#include "Exception.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class TimeAnalyzerNode
// -----------------------------------------------------------------------------

class TimeAnalyzer;

class TimeAnalyzerNode
{
private:
    unsigned int m_depth;
    unsigned int m_index;

    uint64_t m_start;
    uint64_t m_end;
    std::string m_identifier;

    void Clear()
    {
        m_start = 0;
        m_end = 0;
        m_identifier = "";
    }

    // Print the node.
    void Print();

public:
    TimeAnalyzerNode()
    {
        Clear();
    }

    friend class TimeAnalyzer;
};

// -----------------------------------------------------------------------------
// class TimeAnalyzer
// -----------------------------------------------------------------------------

class TimeAnalyzer
{
private:
    std::deque<TimeAnalyzerNode> m_running;
    std::deque<TimeAnalyzerNode> m_done;

    unsigned int m_depth;
    unsigned int m_index;

    TimeAnalyzer() : m_depth(0), m_index(0) {}

public:
    // Get a singleton instance of this class.
    static TimeAnalyzer& Instance();

    void Push(std::string message)
    {
        TimeAnalyzerNode node;

        node.m_identifier = message;
        node.m_start = GetTime::Milli();
        node.m_index = m_index;
        node.m_depth = m_depth;

        m_running.push_back(node);
        m_done.push_back(node);

        m_depth++;
        m_index++;
    }

    void Pop()
    {
        TimeAnalyzerNode back;

        if (m_running.size() == 0)
        {
            throw Exception("Popping empty analyzer queue");
        }

        back = m_running.back();
        m_running.pop_back();

        // Update the node in the done array.
        m_done[back.m_index].m_end = GetTime::Milli();
        m_depth--;
    }

    // Print all the nodes.
    void PrintAnalysis();
};

// -----------------------------------------------------------------------------
// Functions (to simplify calling).
// -----------------------------------------------------------------------------

void PushMessage(std::string message);
void PopMessage();
void PrintAnalysis();

} // end namespace cs

#endif
