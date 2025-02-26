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
//  Last Modified : <250226.0919>
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
#include "openlcb/TractionThrottle.hxx"
#include "openlcb/TrainInterface.hxx"

#include "OpticalLocationSensor.hxx"
#include "Turnout.hxx"
#include "Signal.hxx"
#include "WendellDepot.hxx"
#include <map>

struct RunTrain {
    uint16_t address;
    enum Route {EastBound3,               EastBound1, 
              WestBound4, 
              WestBound2, 
              NUM_ROUTES} route;
    BarrierNotifiable done;
};

typedef StateFlow<Buffer<RunTrain>, QList<1>> RunATrainFlowBase;

class RunATrainFlow : public RunATrainFlowBase
{
public:
    void notify() override {
        LOG(ALWAYS, "*** RunATrainFlow::notify(): %p", this);
        RunATrainFlowBase::notify();
    }
    
    RunATrainFlow(Service *service, openlcb::Node *node);
    void turnout_state(WendellDepot::TurnoutIndexes loc, 
                       Turnout::State_t state,
                       openlcb::EventReport *event,
                       BarrierNotifiable *done);
    void Covered(WendellDepot::SensorIndexes loc,
                 openlcb::EventReport *event,
                 BarrierNotifiable *done);
    void Uncovered(WendellDepot::SensorIndexes loc,
                   openlcb::EventReport *event,
                   BarrierNotifiable *done);
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
        WendellDepot::SensorIndexes terminalLocation;
        RouteTurnoutState turnoutStates[TURNOUT_STATES];
    };
    enum direction_t {Left, Right};
    enum opticalSensorState_t {IsCovered, IsUncovered};
    typedef std::map<WendellDepot::SensorIndexes,RouteSignalState> 
          BlockProtectionSignals_t;
    struct SpeedKey {
        SpeedKey(WendellDepot::SensorIndexes sensor, direction_t dir, 
                 opticalSensorState_t state)
                    : sensor(sensor)
              , dir(dir)
              , state(state)
        {
        }
        WendellDepot::SensorIndexes sensor;
        direction_t dir;
        opticalSensorState_t state;
    };
    enum Speeds_t {
        StopSpeed = 0,
        LowSpeed = 10,
              RestrictedSpeed = 20,
              SlowSpeed = 30,
              ReducedSpeed = 40,
              NormalSpeed = 50};
    struct SpeedKeyCompare {
        bool operator()(const SpeedKey &lhs, const SpeedKey &rhs) const {
            if (lhs.sensor == rhs.sensor)
            {
                if (lhs.dir == rhs.dir)
                {
                    return lhs.state < rhs.state;
                }
                else
                {
                    return lhs.dir < rhs.dir;
                }
            }
            else
            {
                return lhs.sensor < rhs.sensor;
            }
        }
    };
    
    typedef std::map<SpeedKey,openlcb::SpeedType,SpeedKeyCompare> 
          SpeedUpdateMap_t;
private:
    openlcb::Node *node_;
    OpticalLocationSensor *locationSensors_[WendellDepot::NUM_SENSORS];
    Turnout *turnouts_[WendellDepot::NUM_TURNOUTS];
    Signal *signals_[WendellDepot::NUM_SIGNALS];
    RunTrain *currentTrain;
    direction_t currentDirection;
    openlcb::TractionThrottle throttle_;
    openlcb::TractionThrottleInput throttleMessage_;
    const RouteTurnoutList *currentRoute;
    WendellDepot::SensorIndexes terminal;
    uint currentTurnout;
    WendellDepot::TurnoutIndexes turnoutIndx_;
    Turnout::State_t desiredState_;
    uint currentSignal;
    bool atTerminal_;
    virtual Action entry() override;
    Action setTurnout();
    Action waitForPoints();
    Action pointsSetNotify();
    Action pointsSet();
    Action setSignal();
    Action startTrain();
    Action startTrain1();
    Action trainAssigned();
    Action endTrainRun();
    Action trainStopped();
    Action endTrainRun1();
    static const RouteTurnoutList routes_[RunTrain::NUM_ROUTES];
    static const BlockProtectionSignals_t BlockProtectionSignals;
    static const SpeedUpdateMap_t SpeedUpdateMap;
};


#endif // __RUNATRAIN_HXX

