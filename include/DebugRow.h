// -----------------------------------------------------------------------------
// This class serves as a general logging utility for this library.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_DEBUG_DEBUGROW_H
#define API_DEBUG_DEBUGROW_H

namespace cs
{

class AlgorithmDebug;

// -----------------------------------------------------------------------------
// class DebugRow
// -----------------------------------------------------------------------------

class DebugRow
{
private:
    unsigned int m_threshold;
    unsigned int m_confidence;
    unsigned int m_components;
    unsigned int m_textConfidence;
    unsigned int m_textComponents;

public:
    // Constructor.
    DebugRow() :
        m_threshold(0),
        m_confidence(0),
        m_components(0),
        m_textConfidence(0),
        m_textComponents(0)
    {
        // no-op
    }

    std::string ToString() const;

    // Friends
    friend class AlgorithmDebug;
};

} // end namespace cs

#endif
