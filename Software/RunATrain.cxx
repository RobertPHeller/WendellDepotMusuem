// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jan 5 19:21:30 2025
//  Last Modified : <250106.2015>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @copyright
///    Copyright (C) 2025  Robert Heller D/B/A Deepwoods Software
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
/// @file RunATrain.cxx
/// @author Robert Heller
/// @date Sun Jan 5 19:21:30 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include <unistd.h>
#include <type_traits>
#include <functional>
#include <sys/stat.h>

#include "executor/Service.hxx"
#include "executor/Timer.hxx"
#include "utils/Buffer.hxx"
#include "utils/Queue.hxx"
#include "utils/LinkedObject.hxx"
#include "executor/StateFlow.hxx"

#include "OpticalLocationSensor.hxx"
#include "Turnout.hxx"
#include "Signal.hxx"
#include "WendellDepot.hxx"
#include "RunATrain.hxx"

RunATrainFlow::RunATrainFlow(Service *service, openlcb::Node *node)
      : RunATrainFlowBase(service)
, node_(node)
{
    bn_.reset(this);
    for (int i=0; i < WendellDepot::NUM_SENSORS; i++)
    {
        locationSensors_[i] = 
              new OpticalLocationSensor(node_,
                                        WendellDepot::SensorsCfg[i].on,
                                        WendellDepot::SensorsCfg[i].off,
                                        (WendellDepot::SensorIndexes)i,
                                        this);
        locationSensors_[i]->check_sensor(bn_.new_child());
    }
    for (int i=0; i < WendellDepot::NUM_TURNOUTS; i++)
    {
        turnouts_[i] = 
          new Turnout(node_,
                      WendellDepot::TurnoutsCfg[i].motor.normal,
                      WendellDepot::TurnoutsCfg[i].motor.reverse,
                      WendellDepot::TurnoutsCfg[i].points.normal,
                      WendellDepot::TurnoutsCfg[i].points.reverse,
                      (WendellDepot::TurnoutIndexes)i,
                      this);
        turnouts_[i]->check_state(bn_.new_child());
    }
    for (int i=0; i < WendellDepot::NUM_SIGNALS; i++)
    {
        signals_[i] = 
              new Signal(node_,WendellDepot::SignalsCfg[i]);
    }
    bn_.maybe_done();
}

StateFlowBase::Action RunATrainFlow::entry()
{
    return NULL;
}

void RunATrainFlow::turnout_state(WendellDepot::TurnoutIndexes loc, 
                                  Turnout::State_t state)
{
}

void RunATrainFlow::EnterLocation(WendellDepot::SensorIndexes loc)
{
}

void RunATrainFlow::ExitLocation(WendellDepot::SensorIndexes loc)
{
}

