// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jan 5 14:52:37 2025
//  Last Modified : <250213.1546>
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
/// @file RunATrain.hxx
/// @author Robert Heller
/// @date Sun Jan 5 14:52:37 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __RUNATRAIN_HXX
#define __RUNATRAIN_HXX

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


struct RunTrain {
    uint16_t address;
    enum Route {EastBound3, 
              EastBound1, 
              WestBound4, 
              WestBound2, 
              NUM_ROUTES} route;
};

typedef StateFlow<Buffer<RunTrain>, QList<1>> RunATrainFlowBase;

class RunATrainFlow : public RunATrainFlowBase
{
public:
    RunATrainFlow(Service *service, openlcb::Node *node);
    virtual Action entry() override;
    void turnout_state(WendellDepot::TurnoutIndexes loc, 
                       Turnout::State_t state);
    void EnterLocation(WendellDepot::SensorIndexes loc);
    void ExitLocation(WendellDepot::SensorIndexes loc);
private:
    openlcb::Node *node_;
    OpticalLocationSensor *locationSensors_[WendellDepot::NUM_SENSORS];
    Turnout *turnouts_[WendellDepot::NUM_TURNOUTS];
    Signal *signals_[WendellDepot::NUM_SIGNALS];
    RunTrain *currentTrain;
    BarrierNotifiable bn_;
public:
    struct RouteSignalState {
        WendellDepot::SignalIndexes signalIndex;
        WendellDepot::SignalConfig::Aspect signalAspect;
    };
    static constexpr uint SIGNAL_STATES=3;
    struct RouteTurnoutState {
        WendellDepot::TurnoutIndexes turnoutIndex;
        Turnout::State_t state;
        RouteSignalState signalStates[SIGNAL_STATES];
    };
    static constexpr uint TURNOUT_STATES=4;
    struct RouteTurnoutList {
        WendellDepot::SensorIndexes exitLocation;
        RouteTurnoutState turnoutStates[TURNOUT_STATES];
    };
private:
    static const RouteTurnoutList routes_[RunTrain::NUM_ROUTES];
};


#endif // __RUNATRAIN_HXX

