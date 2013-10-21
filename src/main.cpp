#include <iostream>
#include <opencv/highgui.h>

#include "Exception.h"
#include "Algorithm.h"
#include "AlgorithmDebug.h"
#include "AlgorithmTimer.h"

// Program usage.
int usage(char* name, int exitval, std::string message = "")
{
    if (!message.empty())
    {
        std::cout << std::endl << "Error: " << message << std::endl;
    }

    std::cout
        << std::endl
        << "CircuitSense Algorithm: convert an image of an electrical circuit "
        << "to a netlist."
        << std::endl << std::endl
        << "Usage: " << name << " [--debug|--timer|--version|--custom,<x>,<y>] "
        << "<image> "
        << std::endl << std::endl
        << " --debug:   Run the algorithm in debug mode. Verbose output will be"
        << " enabled and"
        << std::endl
        << "            debug images will be saved in the working directory."
        << std::endl
        << " --timer:   Run the algorithm in timer mode. A timing analysis will"
        << " be printed"
        << std::endl
        << "            after the algorithm has finished."
        << std::endl
        << " --version: Print the current software version."
        << std::endl
        << " --custom:  Run with a custom start and end threshold."
        << std::endl << std::endl
        << " <image>:   The circuit image that we are going to convert."
        << std::endl << std::endl
        << "Copyright (c) 2011-2012 by Matt Pillar. All rights reserved."
        << std::endl << std::endl;

    exit(exitval);
}

int main(int argc, char** argv)
{
    // Check options.
    if (argc == 1 || argc > 3)
    {
        usage(argv[0], 1);       
    }

    // Pointers we'll need to clean up.
    std::string srcfile;
    cs::Algorithm* algorithm;
    IplImage* src;

    bool debug = false;
    bool timer = false;
    bool custom = false;

    unsigned int tstart = 0, tend = 0;

    if (argc == 2 &&
        std::string(argv[1]) != "--debug" &&
        std::string(argv[1]) != "--version" &&
        std::string(argv[1]) != "--timer" &&
        std::string(argv[1]).substr(0,9) != "--custom,")
    {
        // No debug.
        srcfile = argv[1];
        src = cvLoadImage(srcfile.c_str());

        if (src == 0)
        {
            std::string message =
                "Could not load source image '" + srcfile + "'.";
            usage(argv[0], 1, message);
        }

        algorithm = new cs::Algorithm(*src);
    }
    else if (std::string(argv[1]) == "--version")
    {
        std::cout
            << std::endl
            << "0.2.0"
            << std::endl << std::endl;
        return 0;
    }
    else if (std::string(argv[1]) == "--debug")
    {
        if (argc != 3)
        {
            usage(argv[0], 1);
        }

        // Running with debug.
        debug = true;
        srcfile = argv[2];
        src = cvLoadImage(srcfile.c_str());

        if (src == 0)
        {
            std::string message =
                "Could not load debug source image '" + srcfile + "'.";
            usage(argv[0], 1, message);
        }

        std::cout << std::endl << "RUNNING IN DEBUG MODE" << std::endl;
        algorithm = new cs::AlgorithmDebug(*src);
    }
    else if (std::string(argv[1]).substr(0,9) == "--custom,")
    {
        if (argc != 3)
        {
            usage(argv[0], 1);
        }

        // Running in custom mode.
        custom = true;
        srcfile = argv[2];
        src = cvLoadImage(srcfile.c_str());

        if (src == 0)
        {
            std::string message =
                "Could not load custom source image '" + srcfile + "'.";
            usage(argv[0], 1, message);
        }

        std::cout
            << std::endl << "RUNNING IN CUSTOM MODE"
            << std::endl;

        algorithm = new cs::Algorithm(*src);

        // parse the thresholds...
        std::string nums = std::string(argv[1]).substr(9,
                std::string(argv[1]).size()-9);
        size_t found = nums.find_first_of(",");

        if (found == std::string::npos)
        {
            usage(argv[0], 1);
        }

        tstart = atoi(nums.substr(0, found).c_str());
        tend = atoi(nums.substr(found+1, nums.size()-found).c_str());
    }
    else
    {
        if (argc != 3)
        {
            usage(argv[0], 1);
        }

        // Running with timer.
        timer = true;
        srcfile = argv[2];
        src = cvLoadImage(srcfile.c_str());

        if (src == 0)
        {
            std::string message =
                "Could not load debug source image '" + srcfile + "'.";
            usage(argv[0], 1, message);
        }

        std::cout
            << std::endl << "RUNNING IN TIMER MODE"
            << std::endl;

        algorithm = new cs::AlgorithmTimer(*src);
    }

    std::cout
        << std::endl << "Executing cs::'s algorithm on " << srcfile
        << "..." << std::endl << std::endl;

    // Execute!
    try
    {
        //uint64_t start = ThreadMaster::GetTime::Milli();

        if (custom)
        {
            algorithm->ConvertToNetlist(tstart, tend);
        }
        else
        {
            algorithm->ConvertToNetlist();
        }

        double elapsed = 0;
        if (debug || timer)
        {
            std::cout << std::endl;
        }

        std::cout
            << "The algorithm took " << elapsed
            << " second(s) to execute." << std::endl
            << "The circuit and text confidences are "
            << algorithm->GetResult().GetConfidence() << "% and "
            << algorithm->GetResult().GetTextConfidence() << "% respectively."
            << std::endl
            << "There are " << algorithm->GetResult().GetNumComponents()
            << " circuit component(s) and "
            << algorithm->GetResult().GetNumTextComponents()
            << " and text value(s)."
            << std::endl << std::endl;
    }
    catch (cs::Exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl << std::endl;
        return 1;
    }

    // Print the output.
    if (algorithm->GetResult().HasErrorText())
    {
        std::cout
            << "Netlist warnings:" << std::endl
            << algorithm->GetResult().GetErrorText() << std::endl << std::endl;
    }

    std::cout
        << "Final Netlist:" << std::endl
        << algorithm->GetResult().GetNetlist() << std::endl << std::endl;

    // Cleanup.
    delete algorithm;
    cvReleaseImage(&src);

    return 0;
}
