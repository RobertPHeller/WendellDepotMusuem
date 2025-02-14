// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Dec 11 09:18:34 2024
//  Last Modified : <250213.1357>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @copyright
///    Copyright (C) 2024  Robert Heller D/B/A Deepwoods Software
///			51 Locke Hill Road
///			Wendell, MA 01379-9728
///
///    This program is free software; you can redistribute it and/or modify
///    it under the terms of the GNU General Public License as published by
///    the Free Software Foundation; either version 2 of the License, or
///    (at your option) any later version.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    You should have received a copy of the GNU General Public License
///    along with this program; if not, write to the Free Software
///    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/// @file main.cxx
/// @author Robert Heller
/// @date Wed Dec 11 09:18:34 2024
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"

#include "WendellDepot.hxx"
#include "OpticalLocationSensor.hxx"
#include "Turnout.hxx"
#include "RunATrain.hxx"
#include "WendellDepotWebserver.hxx"

const openlcb::SimpleNodeStaticValues openlcb::SNIP_STATIC_DATA = {
    4, "Deepwoods Software", "Wendell Depot Musuem", "Linux", "1.00"
};

// Not using a config "file" (no CDI).
extern const char *const openlcb::SNIP_DYNAMIC_FILENAME = nullptr;

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);

// easily incrementable method.
#define DefaultNODEID 0x050101012200ULL // 05 01 01 01 22 00
static openlcb::NodeID NODE_ID = DefaultNODEID;
#define DEFAULT_CAN_SOCKET "can0"
#define DEFAULT_CAN_SERIAL "/dev/ttyACM0"

enum LCCMode {SERVER, GCCLIENT, CANCLIENT, CANSERIAL};
LCCMode lccmode = LCCMode::CANSERIAL;
const char *gchost = "localhost";
const char *cansocket = DEFAULT_CAN_SOCKET;
const char *canserial = DEFAULT_CAN_SERIAL;

// CLI Usage output.

void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-n NID] [-c CAN_SOCKET] [-m mode] [-g host] [-s port]", e);
    fprintf(stderr, "\n\n");
    fprintf(stderr, "Wendell Depot Train Runner.\n\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\t[-n nodeid]\n");
    fprintf(stderr, "\t[-m mode]\n");
    fprintf(stderr, "\t[-g host]\n");
    fprintf(stderr, "\t[-c can_socketname]\n");
    fprintf(stderr, "\t[-s portname]\n");
    fprintf(stderr, "Where:\n");
    fprintf(stderr, "\tnodeid is the node id, as a 12 hex digit number (optionally with colons between pairs of hex digits.\n");
    fprintf(stderr, "\tmode is the LCC connection mode.  One of:\n");
    fprintf(stderr, "\t\tserver - start a GC hub (host 0.0.0.0, port = 12021)\n");
    fprintf(stderr, "\t\tgcclient - connect a GC hub (at the host specificed by -h, port 12021)\n");
    fprintf(stderr, "\t\tcanclient - connect a CAN Socket (-c)\n");
    fprintf(stderr, "\t\tcanserial - connect a USB Serial (GC) port (-s) (default)\n");
    fprintf(stderr, "\thost the GC hub server to connect to\n");
    fprintf(stderr, "\tcan_socketname the CAN socket\n");
    fprintf(stderr, "\tportname the USB Serial port\n");
    exit(1);
}

// Parse CLI options.

openlcb::NodeID parseNodeID(const char *nidstring)
{
    uint64_t result = 0ULL;
    int nibcount = 0, coloncount = 0;
    const char *p = NULL;
    for (p = nidstring; *p != '\0'; p++)
    {
        if (isxdigit(*p))
        {
            nibcount++;
            if (isdigit(*p))
            {
                result = (result<<4)+(*p-'0');
            }
            else if (islower(*p))
            {
                result = (result<<4)+(*p-'a'+10);
            }
            else
            {
                result = (result<<4)+(*p-'A'+10);
            }
        }
        else if (*p == ':')
        {
            coloncount++;
        }
        else
        {
            // not a hex digit or colon
            fprintf(stderr, "Syntax error: Illformed node id: %s\n",nidstring);
            return (openlcb::NodeID) -1;
        }
    }
    if (nibcount != 12)
    {
        // Wrong number of digits
        fprintf(stderr, "Syntax error: Illformed node id: %s\n",nidstring);
        return (openlcb::NodeID) -1;
    }
    if (coloncount != 0 && coloncount != 5)
    {
        // Wrong number of colons (some number other than 0 or 5)
        fprintf(stderr, "Syntax error: Illformed node id: %s\n",nidstring);
        return (openlcb::NodeID) -1;
    }
    return (openlcb::NodeID) result;
}


void parse_args(int argc, char *argv[])
{
    int opt;
#define OPTSTRING "hn:c:m:g:s:"
    while ((opt = getopt(argc, argv, OPTSTRING)) >= 0)
    {
        switch (opt)
        {
        case 'h':
            usage(argv[0]);
            break;
        case 'n':
            {
                openlcb::NodeID nid = parseNodeID(optarg);
                if (((int64_t)nid) == -1) 
                {
                    usage(argv[0]);
                }
                else
                {
                    NODE_ID = nid;
                }
            }
            break;
        case 'c':
            cansocket = optarg;
            break;
        case 'g':
            gchost = optarg;
            break;
        case 'm':
            if (strncasecmp(optarg,"server",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::SERVER;
            }
            else if (strncasecmp(optarg,"gcclient",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::GCCLIENT;
            }
            else if (strncasecmp(optarg,"canclient",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::CANCLIENT;
            }
            else if (strncasecmp(optarg,"canserial",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::CANSERIAL;
            }
            else
            {
                fprintf(stderr, "Unknown mode: %s\n",optarg);
                usage(argv[0]);
            }
            break;
        case 's':
            canserial = optarg;
            break;
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            usage(argv[0]);
        }
    }
}

int appl_main(int argc, char *argv[])
{
    // Parse command line.
    parse_args(argc, argv);
    
    openlcb::SimpleCanStack stack(NODE_ID);
#ifdef PRINT_ALL_PACKETS
    // Causes all packets to be dumped to stdout.
    stack.print_all_packets();
#endif
    switch (lccmode) {
    case LCCMode::SERVER:
        stack.start_tcp_hub_server(12021);
        break;
    case LCCMode::GCCLIENT:
        stack.connect_tcp_gridconnect_hub(gchost,12021);
        break;
    case LCCMode::CANCLIENT:
        stack.add_socketcan_port_select(cansocket);
        break;
    case LCCMode::CANSERIAL:
        stack.add_gridconnect_port(canserial);
        break;
    }
    RunATrainFlow runtrains(stack.service(),stack.node());
    Executor<1> httpd_executor("httpd_executor", 0, 1024);
    WendellDepotWebserver webserver(&httpd_executor,8080,"/home/heller/WendellDepotMusuem/Software/static");
    stack.loop_executor();
    return 0;
}

