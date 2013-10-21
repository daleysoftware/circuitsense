// -----------------------------------------------------------------------------
// Implementation of the circuit library. This particular class is used to
// convert a Circuit object to a netlist.
//
// This file and the component aggregate implementation file are the only files
// you need to change when you add a circuit component to the tesseract training
// library (when you are using the new infrastructure algorithm, i.e. the 
// infrastructure).
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <opencv/cv.h>

#include "NetlistGenerator.h"
#include "Circuit.h"

namespace cs
{

static void SetUnknownValueIfEmpty(CircuitEntity& entity)
{
    if (entity.GetValue().empty())
    {
        entity.SetValue("???");
    }
}

// Utility to help write port statements and log errors.
void NetlistGenerator::WritePortStatement(
        CircuitEntity& entity,
        unsigned int componentNumber,
        const std::string& netlistIdentifier,
        const std::vector<unsigned int>& vec)
{
    unsigned int sequential = entity.GetNumSequentialConnections();
    if (sequential != vec.size())
    {
        m_error
            << "Component with ID '" << entity.GetComponentID().GetID()
            << "' has " << sequential << " sequential connection(s), expected "
            << vec.size() << "." << std::endl;
    }

    m_netlist << netlistIdentifier << componentNumber;

    for (std::vector<unsigned int>::const_iterator it = vec.begin();
            it != vec.end(); it++)
    {
        CircuitEntityConnection* conn = entity.GetConnectionByPort(*it);
        if (conn == 0)
        {
            m_netlist << " ???";
        }
        else
        {
            m_netlist << " " << conn->GetNode();
        }
    }

    if (entity.HasValue())
    {
        m_netlist << " " << entity.GetValue();
    }

    m_netlist << std::endl;
}

void NetlistGenerator::Write2PortStatement(
        CircuitEntity& entity,
        unsigned int componentNumber,
        std::string netlistIdentifier,
        unsigned int port1,
        unsigned int port2)
{
    std::vector<unsigned int> vec;

    vec.push_back(port1);
    vec.push_back(port2);

    WritePortStatement(entity, componentNumber, netlistIdentifier, vec);
}

void NetlistGenerator::Write3PortStatement(
        CircuitEntity& entity,
        unsigned int componentNumber,
        std::string netlistIdentifier,
        unsigned int port1,
        unsigned int port2,
        unsigned int port3)
{
    std::vector<unsigned int> vec;

    vec.push_back(port1);
    vec.push_back(port2);
    vec.push_back(port3);

    WritePortStatement(entity, componentNumber, netlistIdentifier, vec);
}

void NetlistGenerator::ConvertToNetlistInternal()
{
    // Make a copy of the circuit object for use in this function. We're going
    // to perform removes and sorts and we don't want to muck with the caller's
    // object.
    m_modified = m_circuit;

    // Find the grounds and set any nodes that touch ground to be the special
    // case zero node. Delete the ground once the switch has been made.
    bool more;
    do
    {
        more = false;
        for (Circuit::EntityVector::iterator it =
                m_modified.m_entities.begin();
                it != m_modified.m_entities.end(); it++)
        {
            if (it->GetComponentID().IsGround())
            {
                // This is similar to the remove function, except that instead
                // of deleting stray references we just set them to zero.
                while (it->GetNumConnections() > 0)
                {
                    CircuitEntityConnection conn = it->RemoveOne();
                    m_modified.ReplaceNodeNumber(conn.GetNode(), 0);
                }

                // Now we can delete the entity. Break out and start over
                // because this iterator is not invalid.
                m_modified.m_entities.erase(it);
                more = true;
                break;
            }
        }
    }
    while (more);

    // Delete the null entities (these have been converted to text or are
    // actually nodes).
    for (unsigned int i = 0; i < m_modified.GetNumEntities(); i++)
    {
        if (m_modified.m_entities[i].GetComponentID().IsNull())
        {
            m_modified.RemoveEntity(i);
            i--;
        }
    }

    // Ground nodes and null entries have now been removed. Sort the entities.
    m_modified.Sort();
    m_modified.FixNodeNumbers();

    // Sort the node numbers. Should not need to fix node numbers afterward.
    m_modified.SortNodes();

    // Used to count consecutive component types for number the netlist.
    unsigned int componentNumber = 0;
    SmartComponentID prevID(0);

    // Go though each entity (component) and write the output statements.
    for (Circuit::EntityVector::iterator it =
            m_modified.m_entities.begin();
            it != m_modified.m_entities.end(); it++)
    {
        char id = it->GetComponentID().GetID();

        // Update the previous ID and the counter.
        if (prevID == it->GetComponentID())
        {
            componentNumber++;
        }
        else
        {
            componentNumber = 0;
        }
        prevID = it->GetComponentID();

        switch (id)
        {
            // -----------------------------------------------------------------
            // Start component specific handling. New components must be added
            // here.
            // -----------------------------------------------------------------

            // Resistors
            case 'R':
            case 'r':
            {
                SetUnknownValueIfEmpty(*it);
                Write2PortStatement(*it, componentNumber, "R", 1, 0);
                break;
            }

            // Inductors
            case 'L':
            case 'l':
            {
                SetUnknownValueIfEmpty(*it);
                Write2PortStatement(*it, componentNumber, "L", 1, 0);
                break;
            }

            // Capacitors
            case 'C':
            case 'c':
            {
                SetUnknownValueIfEmpty(*it);
                Write2PortStatement(*it, componentNumber, "C", 1, 0);
                break;
            }

            // Signal Generators
            case 'S':
            case 's':
            {
                SetUnknownValueIfEmpty(*it);
                std::ostringstream os;
                os << "AC SIN(0 " << it->GetValue() << " 60 0 0)";
                it->SetValue(os.str());
                Write2PortStatement(*it, componentNumber, "V",
                        id == 'S' ? 0 : 1,
                        id == 'S' ? 1 : 0);
                break;
            }

            // DC voltage sources
            case 'V':
            case 'a':
            case 'v':
            case 'A':
            {
                SetUnknownValueIfEmpty(*it);
                std::ostringstream os;
                os << "DC " << it->GetValue();
                it->SetValue(os.str());
                Write2PortStatement(*it, componentNumber, "V",
                        id == 'V' || id == 'a' ? 1 : 0,
                        id == 'V' || id == 'a' ? 0 : 1);
                break;
            }

            // Diodes
            case 'D':
            case 'e':
            case 'd':
            case 'E':
            {
                std::ostringstream os1, os2;
                os1 << ".MODEL DIODE" << componentNumber << " D" << std::endl;
                os1 << "D";
                os2 << "DIODE" << componentNumber << " " << it->GetValue();
                it->SetValue(os2.str());
                Write2PortStatement(*it, componentNumber, os1.str(),
                        id == 'D' || id == 'e' ? 1 : 0,
                        id == 'D' || id == 'e' ? 0 : 1);
                break;
            }

            // NPN BJT's
            case 'N':
            case 'n':
            {
                std::ostringstream os1, os2;
                os1 << ".MODEL BJT" << componentNumber << " NPN" << std::endl;
                os1 << "Q";
                os2 << "BJT" << componentNumber << " " << it->GetValue();
                it->SetValue(os2.str());
                Write3PortStatement(*it, componentNumber, os1.str(),
                        id == 'N' ? 0 : 2,
                        id == 'N' ? 2 : 0,
                        1);
                break;
            }

            // PNP BJT's
            case 'P':
            case 'p':
            {
                std::ostringstream os1, os2;
                os1 << ".MODEL BJT" << componentNumber << " PNP" << std::endl;
                os1 << "Q";
                os2 << "BJT" << componentNumber << " " << it->GetValue();
                it->SetValue(os2.str());
                Write3PortStatement(*it, componentNumber, os1.str(),
                        1,
                        id == 'P' ? 2 : 0,
                        id == 'p' ? 0 : 2);
                break;
            }

            // NMOS Transistors
            case 'T':
            case 't':
            {
                std::ostringstream os1, os2;
                os1 << ".MODEL MOS" << componentNumber << " NMOS" << std::endl;
                os1 << "M";
                os2 << "MOS" << componentNumber << " " << it->GetValue();
                it->SetValue(os2.str());
                Write3PortStatement(*it, componentNumber, os1.str(),
                        id == 'T' ? 0 : 2,
                        id == 'T' ? 2 : 0,
                        1);
                break;
            }

            // PMOS Transistors
            case 'Q':
            case 'q':
            {
                std::ostringstream os1, os2;
                os1 << ".MODEL MOS" << componentNumber << " PMOS" << std::endl;
                os1 << "M";
                os2 << "MOS" << componentNumber << " " << it->GetValue();
                it->SetValue(os2.str());
                Write3PortStatement(*it, componentNumber, os1.str(),
                        1,
                        id == 'P' ? 2 : 0,
                        id == 'p' ? 0 : 2);
                break;
            }

            // -----------------------------------------------------------------
            // End component specific handling.
            // -----------------------------------------------------------------

            default:
            {
                m_error
                    << "Unknown component ID '"
                    << it->GetComponentID().GetID() << "'" << std::endl;
                break;
            }
        }
    }

    // Default footer.
    m_netlist << std::endl;
    m_netlist << ".CONTROL" << std::endl;
    m_netlist << "ac dec 10 60 600k" << std::endl;
    m_netlist << "op" << std::endl;
    m_netlist << "tran 1us 10us 0" << std::endl;
    m_netlist << "print v(1)" << std::endl;
    m_netlist << ".ENDC" << std::endl;
    m_netlist << ".END" << std::endl;
}

std::string NetlistGenerator::GetNetlist() const
{
    if (!m_netlist.str().empty())
    {
        return m_netlist.str().substr(0, m_netlist.str().size()-1);
    }

    return std::string();
}

std::string NetlistGenerator::GetErrorText() const
{
    if (!m_error.str().empty())
    {
        return m_error.str().substr(0, m_error.str().size()-1);
    }

    return std::string();
}

std::string NetlistGenerator::GetDefaultNetlistText()
{
    Circuit circuit;

    NetlistGenerator generator(circuit);
    generator.ConvertToNetlist();

    std::ostringstream os;
    os << std::endl << generator.GetNetlist();

    return os.str();
}

} // end namespace cs
