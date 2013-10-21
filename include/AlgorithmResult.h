// -----------------------------------------------------------------------------
// This header contains the public prototypes for the algorithm library.
// The algorithm library brings all the other  libraries together; it
// takes an opencv image as an input and the output is a netlist.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <string>

#ifndef API_ALGORITHM_ALGORITHMRESULTH
#define API_ALGORITHM_ALGORITHMRESULTH

namespace cs
{

// -----------------------------------------------------------------------------
// class AlgorithmResult
// -----------------------------------------------------------------------------

class AlgorithmBase;

class __attribute__ ((visibility ("default"))) AlgorithmResult
{
private:
    // The resulting netlist.
    std::string m_netlist;

    // The warning and error text from the netlist generator.
    std::string m_error;

    // The best threshold.
    unsigned int m_threshold;
    unsigned int m_textThreshold;

    // The best confidence. This value is calculated by averaging the confidence
    // values of all the valid components in the circuit.
    unsigned int m_confidence;
    unsigned int m_textConfidence;

    // The number of not-null connected entities in the circuit object.
    unsigned int m_components;
    unsigned int m_textComponents;

public:
    // Constructor.
    AlgorithmResult() :
        m_threshold(0),
        m_textThreshold(0),
        m_confidence(0),
        m_textConfidence(0),
        m_components(0),
        m_textComponents(0)
    {
        // no-op
    }

    // Return the netlist. If ConvertToNetlist has not been called yet, return
    // an empty string.
    const std::string& GetNetlist() const {return m_netlist;}

    // Any error or warning messages that result from netlist generation.
    const std::string& GetErrorText() const {return m_error;}

    // Return true if there were any warnings generated during netlist
    // conversion.
    bool HasErrorText() const {return !m_error.empty();}

    // Returns the threshold that we determined was most optimal for this
    // particular circuit image.
    unsigned int GetBestThreshold() const {return m_threshold;}
    unsigned int GetBestTextThreshold() const {return m_textThreshold;}

    // A confidence metric that describes how sure we are that we parsed the
    // circuit correctly.
    unsigned int GetConfidence() const {return m_confidence;}
    unsigned int GetTextConfidence() const {return m_textConfidence;}

    // Get the number of components we found in the resulting circuit.
    unsigned int GetNumComponents() const {return m_components;}
    unsigned int GetNumTextComponents() const {return m_textComponents;}

    // Friends
    friend class AlgorithmBase;
};

} // end namespace cs

#endif
