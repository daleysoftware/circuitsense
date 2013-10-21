// -----------------------------------------------------------------------------
// This header contains the public prototypes for the circuit library.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_CIRCUIT_NETLISTGENERATOR_H
#define API_CIRCUIT_NETLISTGENERATOR_H

#include <string>
#include <sstream>

#include "CircuitEntityConnection.h"
#include "CircuitEntity.h"
#include "Circuit.h"

namespace cs
{

// -----------------------------------------------------------------------------
// class NetlistGenerator
// -----------------------------------------------------------------------------

class NetlistGenerator
{
private:
    // The circuit that we are going to convert.
    const Circuit& m_circuit;

    // The modified circuit object, after netlist generation.
    Circuit m_modified;

    // Boolean indicating whether or not we have already converted the netlist.
    // This is only ever done once per instance.
    bool m_done;

    // Strings for the results.
    std::ostringstream m_netlist;
    std::ostringstream m_error;

    // Utilities for writing netlist output statements.
    void WritePortStatement(CircuitEntity& entity,
            unsigned int componentNumber, const std::string& netlistIdentifier,
            const std::vector<unsigned int>& vec);

    void Write2PortStatement(CircuitEntity& entity,
            unsigned int componentNumber, std::string netlistIdentifier,
            unsigned int port1, unsigned int port2);

    void Write3PortStatement(CircuitEntity& entity,
            unsigned int componentNumber, std::string netlistIdentifier,
            unsigned int port1, unsigned int port2, unsigned int port3);

    // Called by ConvertToNetlist().
    void ConvertToNetlistInternal();

public:
    // Constructor.
    NetlistGenerator(const Circuit& circuit) :
        m_circuit(circuit),
        m_done(false)
    {
        // no-op
    }

    // Convert the circuit to a netlist. This will not modify the orignal
    // circuit object that was passed in.
    void ConvertToNetlist()
    {
        if (!m_done)
        {
            ConvertToNetlistInternal();
            m_done = true;
        }
    }

    // Get the resulting netlist.
    std::string GetNetlist() const;

    // Any error or warning messages that result from netlist generation.
    std::string GetErrorText() const;

    // Return true if there were any warnings generated during netlist
    // conversion.
    bool HasErrorText() const {return !m_error.str().empty();}

    // Get the modified netlist that results from circuit generation.
    const Circuit& GetModifiedCircuit() const {return m_modified;}

    // Get the default netlist text.
    static std::string GetDefaultNetlistText();
};

} // end namespace cs

#endif
