// -----------------------------------------------------------------------------
// Copyright (c) 2011 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TESS_RESULTCIRCUITS_H
#define API_TESS_RESULTCIRCUITS_H

#include <vector>
#include "ComponentID.h"

namespace cs
{

// Forward Declarations.
class Tesseract;

// -----------------------------------------------------------------------------
// class ResultCircuits
// -----------------------------------------------------------------------------

class ResultCircuits
{
public:
    typedef std::vector<ComponentID> CircuitVector;

private:
    // Members
    CircuitVector m_vector;
    int m_confidence;

public:
    // Constructor
    ResultCircuits() : m_confidence(0)
    {
        // no-op
    }

    // Accessors
    int GetConfidence() const {return m_confidence;}
    const CircuitVector& GetVector() const {return m_vector;}
    std::string ToString() const {return VectorToString(m_vector);}

    // Statics
    static std::string VectorToString(const CircuitVector& vec);

    // Friends
    friend class Tesseract;
};

} // end namespace cs

#endif
