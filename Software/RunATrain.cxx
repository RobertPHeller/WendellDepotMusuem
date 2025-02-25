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
//  Last Modified : <250225.1432>
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

const RunATrainFlow::RouteTurnoutList RunATrainFlow::routes_[RunTrain::NUM_ROUTES] = 
{
    {
        WendellDepot::West_Stage_Exit_3,
        WendellDepot::East_Stage_Exit_3,
        {
            {
                WendellDepot::Stage_West_1_3,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::WestYardEntry1_3,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::WestExit1__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    },
                    {
                        WendellDepot::WestExit3__Dwarf_,
                        WendellDepot::SignalConfig::Go
                    }
                }
            },
            {
                WendellDepot::West_Double_Track,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::WestEN,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::WestF1,
                        WendellDepot::SignalConfig::Slow_Clear
                    },
                    {
                        WendellDepot::WestFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::East_Double_Track,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::EastEN,
                        WendellDepot::SignalConfig::Clear
                    },
                    {
                        WendellDepot::EastF1,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::EastFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::Stage_East_1_3,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::EastYardEntry1_3,
                        WendellDepot::SignalConfig::Slow_Clear
                    },
                    {
                        WendellDepot::EastExit1__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    },
                    {
                        WendellDepot::EastExit3__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    }
                }
            }
        }
    },
    {
        WendellDepot::West_Stage_Exit_1,
        WendellDepot::East_Stage_Exit_1,
        {
            {
                WendellDepot::Stage_West_1_3,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::WestYardEntry1_3,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::WestExit1__Dwarf_,
                        WendellDepot::SignalConfig::Go
                    },
                    {
                        WendellDepot::WestExit3__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    }
                }
            },
            {
                WendellDepot::West_Double_Track,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::WestEN,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::WestF1,
                        WendellDepot::SignalConfig::Slow_Clear
                    },
                    {
                        WendellDepot::WestFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::East_Double_Track,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::EastEN,
                        WendellDepot::SignalConfig::Clear
                    },
                    {
                        WendellDepot::EastF1,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::EastFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::Stage_East_1_3,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::EastYardEntry1_3,
                        WendellDepot::SignalConfig::Clear
                    },
                    {
                        WendellDepot::EastExit1__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    },
                    {
                        WendellDepot::EastExit3__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    }
                }
            }
        }
    },
    {
        WendellDepot::East_Stage_Exit_4,
        WendellDepot::West_Stage_Exit_4,
        {
            {
                WendellDepot::Stage_East_2_4,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::EastYardEntry2_4,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::EastExit2__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    },
                    {
                        WendellDepot::EastExit4__Dwarf_,
                        WendellDepot::SignalConfig::Go
                    }
                }
            },
            {
                WendellDepot::East_Double_Track,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::EastEN,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::EastF1,
                        WendellDepot::SignalConfig::Slow_Clear
                    },
                    {
                        WendellDepot::EastFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::West_Double_Track,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::WestEN,
                        WendellDepot::SignalConfig::Clear
                    },
                    {
                        WendellDepot::WestF1,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::WestFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::Stage_West_2_4,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::WestYardEntry2_4,
                        WendellDepot::SignalConfig::Slow_Clear
                    },
                    {
                        WendellDepot::WestExit2__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    },
                    {
                        WendellDepot::WestExit4__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    }
                }
            }
        }
    },
    {
        WendellDepot::East_Stage_Exit_2,
        WendellDepot::West_Stage_Exit_2,
        {
            {
                WendellDepot::Stage_East_2_4,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::EastYardEntry2_4,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::EastExit2__Dwarf_,
                        WendellDepot::SignalConfig::Go
                    },
                    {
                        WendellDepot::EastExit4__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    }
                }
            },
            {
                WendellDepot::East_Double_Track,
                Turnout::REVERSE,
                {
                    {
                        WendellDepot::EastEN,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::EastF1,
                        WendellDepot::SignalConfig::Slow_Clear
                    },
                    {
                        WendellDepot::EastFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::West_Double_Track,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::WestEN,
                        WendellDepot::SignalConfig::Clear
                    },
                    {
                        WendellDepot::WestF1,
                        WendellDepot::SignalConfig::Stop
                    },
                    {
                        WendellDepot::WestFM,
                        WendellDepot::SignalConfig::Stop
                    }
                }
            },
            {
                WendellDepot::Stage_West_2_4,
                Turnout::NORMAL,
                {
                    {
                        WendellDepot::WestYardEntry2_4,
                        WendellDepot::SignalConfig::Clear
                    },
                    {
                        WendellDepot::WestExit2__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    },
                    {
                        WendellDepot::WestExit4__Dwarf_,
                        WendellDepot::SignalConfig::Hold
                    }
                }
            }
        }
    }
};

const RunATrainFlow::BlockProtectionSignals_t 
          RunATrainFlow::BlockProtectionSignals = {
{WendellDepot::West_Stage_Exit_3, {WendellDepot::WestExit3__Dwarf_,WendellDepot::SignalConfig::Hold}},
{WendellDepot::West_Stage_Exit_1, {WendellDepot::WestExit1__Dwarf_,WendellDepot::SignalConfig::Hold}},
{WendellDepot::West_Stage_Exit_4, {WendellDepot::WestExit4__Dwarf_,WendellDepot::SignalConfig::Hold}},
{WendellDepot::West_Stage_Exit_2, {WendellDepot::WestExit2__Dwarf_,WendellDepot::SignalConfig::Hold}},
{WendellDepot::Stage_West_1_3_Entrance, {WendellDepot::WestYardEntry1_3, WendellDepot::SignalConfig::Stop}},
{WendellDepot::Stage_West_2_4_Entrance, {WendellDepot::WestYardEntry2_4, WendellDepot::SignalConfig::Stop}},
{WendellDepot::Stage_East_1_3_Entrance, {WendellDepot::EastYardEntry1_3, WendellDepot::SignalConfig::Stop}},
{WendellDepot::Stage_East_2_4_Entrance, {WendellDepot::EastYardEntry2_4, WendellDepot::SignalConfig::Stop}},
{WendellDepot::West_Double_Track_Entrance, {WendellDepot::WestEN, WendellDepot::SignalConfig::Stop}},
{WendellDepot::West_Double_Track_Straight_Stop, {WendellDepot::WestFM, WendellDepot::SignalConfig::Stop}},
{WendellDepot::West_Double_Track_Frog_Stop, {WendellDepot::WestFM, WendellDepot::SignalConfig::Stop}},
{WendellDepot::East_Double_Track_Entrance, {WendellDepot::EastEN, WendellDepot::SignalConfig::Stop}},
{WendellDepot::East_Double_Track_Straight_Stop, {WendellDepot::EastFM, WendellDepot::SignalConfig::Stop}},
{WendellDepot::East_Double_Track_Frog_Stop, {WendellDepot::EastFM, WendellDepot::SignalConfig::Stop}},
};

const RunATrainFlow::SpeedUpdateMap_t RunATrainFlow::SpeedUpdateMap = {
    {{WendellDepot::West_Stage_Exit_3, RunATrainFlow::Right, RunATrainFlow::IsCovered},
        {RunATrainFlow::SlowSpeed}},
    {{WendellDepot::West_Stage_Exit_1, RunATrainFlow::Right, RunATrainFlow::IsCovered},
        {RunATrainFlow::ReducedSpeed}},
    {{WendellDepot::West_Double_Track_Frog_Stop, RunATrainFlow::Right, RunATrainFlow::IsCovered},
        {RunATrainFlow::RestrictedSpeed}},
    {{WendellDepot::West_Double_Track_Entrance, RunATrainFlow::Right, RunATrainFlow::IsUncovered},
        {RunATrainFlow::NormalSpeed}},
    {{WendellDepot::East_Double_Track_Straight_Stop, RunATrainFlow::Right, RunATrainFlow::IsCovered},
        {RunATrainFlow::ReducedSpeed}},
    {{WendellDepot::East_Double_Track_Straight_Stop, RunATrainFlow::Right, RunATrainFlow::IsUncovered},
        {RunATrainFlow::SlowSpeed}},
    {{WendellDepot::Stage_East_1_3_Entrance, RunATrainFlow::Right, RunATrainFlow::IsCovered},
        {RunATrainFlow::LowSpeed}},
    {{WendellDepot::East_Stage_Exit_3, RunATrainFlow::Right, RunATrainFlow::IsUncovered},
        {RunATrainFlow::StopSpeed}},
    {{WendellDepot::East_Stage_Exit_1, RunATrainFlow::Right, RunATrainFlow::IsUncovered},
        {RunATrainFlow::StopSpeed}},
    {{WendellDepot::East_Stage_Exit_4, RunATrainFlow::Left, RunATrainFlow::IsCovered},
        {RunATrainFlow::SlowSpeed}},
    {{WendellDepot::East_Stage_Exit_2, RunATrainFlow::Left, RunATrainFlow::IsCovered},
        {RunATrainFlow::ReducedSpeed}},
    {{WendellDepot::East_Double_Track_Frog_Stop, RunATrainFlow::Left, RunATrainFlow::IsCovered},
        {RunATrainFlow::RestrictedSpeed}},
    {{WendellDepot::East_Double_Track_Entrance, RunATrainFlow::Left, RunATrainFlow::IsUncovered},
        {RunATrainFlow::NormalSpeed}},
    {{WendellDepot::West_Double_Track_Straight_Stop, RunATrainFlow::Left, RunATrainFlow::IsCovered},
        {RunATrainFlow::ReducedSpeed}},
    {{WendellDepot::West_Double_Track_Straight_Stop, RunATrainFlow::Left, RunATrainFlow::IsUncovered},
        {RunATrainFlow::SlowSpeed}},
    {{WendellDepot::Stage_West_2_4_Entrance, RunATrainFlow::Left, RunATrainFlow::IsCovered},
        {RunATrainFlow::LowSpeed}},
    {{WendellDepot::West_Stage_Exit_4, RunATrainFlow::Left, RunATrainFlow::IsUncovered},
        {RunATrainFlow::StopSpeed}},
    {{WendellDepot::West_Stage_Exit_2, RunATrainFlow::Left, RunATrainFlow::IsUncovered},
        {RunATrainFlow::StopSpeed}},
};

RunATrainFlow::RunATrainFlow(Service *service, openlcb::Node *node)
      : RunATrainFlowBase(service)
, node_(node)
, currentTrain(nullptr)
, throttle_(node_)
, currentRoute(nullptr)
, terminal(WendellDepot::NUM_SENSORS)
, currentTurnout(RunATrainFlow::TURNOUT_STATES)
, turnoutIndx_(WendellDepot::NUM_TURNOUTS)
, desiredState_(Turnout::UNKNOWN)
, currentSignal(RunATrainFlow::SIGNAL_STATES)
{
    for (int i=0; i < WendellDepot::NUM_SENSORS; i++)
    {
        locationSensors_[i] = 
              new OpticalLocationSensor(node_,
                                        WendellDepot::SensorsCfg[i].on,
                                        WendellDepot::SensorsCfg[i].off,
                                        (WendellDepot::SensorIndexes)i,
                                        this);
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
    }
    for (int i=0; i < WendellDepot::NUM_SIGNALS; i++)
    {
        signals_[i] = 
              new Signal(node_,WendellDepot::SignalsCfg[i]);
    }
}

StateFlowBase::Action RunATrainFlow::entry()
{
    LOG(ALWAYS,"*** RunATrainFlow::entry()");
    currentTrain = message()->data();
    currentRoute = &routes_[(uint)(currentTrain->route)];
    terminal = currentRoute->terminalLocation;
    currentTurnout = 0;
    if (currentTrain->route == RunTrain::EastBound1 || 
        currentTrain->route == RunTrain::EastBound3)
    {
        currentDirection = Right;
    }
    else
    {
        currentDirection = Left;
    }
    LOG(ALWAYS,"*** RunATrainFlow::entry(): currentTrain->address = %d",(int)(currentTrain->address));
    LOG(ALWAYS,"*** RunATrainFlow::entry(): currentRoute is %d",(int)(currentTrain->route));
    LOG(ALWAYS,"*** RunATrainFlow::entry(): currentDirection = %d",currentDirection);
    return call_immediately(STATE(setTurnout));
}

StateFlowBase::Action RunATrainFlow::setTurnout()
{
    LOG(ALWAYS,"*** RunATrainFlow::setTurnout() currentTurnout is %d",currentTurnout);
    if (currentTurnout == TURNOUT_STATES) // end of turnouts to set for route
    {
        // all turnouts and signals set, go start the train
        return call_immediately(STATE(startTrain));
    }
    else
    {
        const RouteTurnoutState *ct = &currentRoute->turnoutStates[currentTurnout];
        LOG(ALWAYS,"*** RunATrainFlow::setTurnout(): ct->turnoutIndex is %d",ct->turnoutIndex);
        LOG(ALWAYS,"*** RunATrainFlow::setTurnout(): ct->state is %d",ct->state);
        turnoutIndx_ = ct->turnoutIndex;
        desiredState_ = ct->state;
        LOG(ALWAYS,"*** RunATrainFlow::setTurnout(): turnouts_[turnoutIndx_]->State() is %d",turnouts_[turnoutIndx_]->State());
        // Are points already set?
        if (desiredState_ == turnouts_[turnoutIndx_]->State())
        {
            // Yes, go set the signals
            return call_immediately(STATE(pointsSet));
        }
        else
        {
            // Send event to move the points.
            LOG(ALWAYS,"*** RunATrainFlow::setTurnout(): About to send event to move points");
            //BarrierNotifiable bn(this);
            switch (desiredState_)
            {
            case Turnout::NORMAL:
                turnouts_[(uint)(ct->turnoutIndex)]->Normal(this);
                break;
            case Turnout::REVERSE:
                turnouts_[(uint)(ct->turnoutIndex)]->Reverse(this);
                break;
            default:
                break;
            }
            //bn.maybe_done();
            LOG(ALWAYS,"*** RunATrainFlow::setTurnout(): event sent");
            return wait_and_call(STATE(waitForPoints));
        }
    }
}

StateFlowBase::Action RunATrainFlow::waitForPoints()
{
    LOG(ALWAYS,"*** RunATrainFlow::waitForPoints(): desiredState is %d",desiredState_);
    /* something goes here to cause things to wait (sleep?) */
    return wait_and_call(STATE(pointsSetNotify));
}

StateFlowBase::Action RunATrainFlow::pointsSetNotify()
{
    LOG(ALWAYS,"*** RunATrainFlow::pointsSetNotify()");
    return call_immediately(STATE(pointsSet));
}

StateFlowBase::Action RunATrainFlow::pointsSet()
{
    LOG(ALWAYS,"*** RunATrainFlow::pointsSet()");
    // Points moved, start setting the signals around the turnout
    currentSignal = 0;
    return call_immediately(STATE(setSignal));
}

StateFlowBase::Action RunATrainFlow::setSignal()
{
    LOG(ALWAYS,"*** RunATrainFlow::setSignal(): currentSignal = %d",currentSignal);
    if (currentSignal == SIGNAL_STATES)
    {
        // finished with this turnouts signals, go to the next turnout in the route.
        currentTurnout++;
        return call_immediately(STATE(setTurnout));
    }
    else
    {
        // set a signal
        //BarrierNotifiable bn(this);
        const RouteTurnoutState *ct = &currentRoute->turnoutStates[currentTurnout];
        const RouteSignalState *cs = &(ct->signalStates[currentSignal]);
        signals_[(uint)(cs->signalIndex)]->SetAspect(cs->signalAspect,this);
        //bn.maybe_done();
        currentSignal++; // next signal
        return wait();
    }
}

StateFlowBase::Action RunATrainFlow::startTrain()
{
    LOG(ALWAYS,"*** RunATrainFlow::startTrain()");
    currentTrain->done.notify();
    currentTrain = nullptr;
    currentRoute = nullptr;
    terminal = WendellDepot::NUM_SENSORS;
    currentTurnout = RunATrainFlow::TURNOUT_STATES;
    turnoutIndx_ = WendellDepot::NUM_TURNOUTS;
    desiredState_ = Turnout::UNKNOWN;
    currentSignal = RunATrainFlow::SIGNAL_STATES;
    return release_and_exit();
    //return allocate_and_call(&throttle_,STATE(startTrain1));
}

StateFlowBase::Action RunATrainFlow::startTrain1()
{
    openlcb::NodeID train = 0x06010000C000 | currentTrain->address;
    LOG(ALWAYS,"*** RunATrainFlow::startTrain1(): train = 0X%012lX",train);
    Buffer<openlcb::TractionThrottleInput> *buffer = 
          get_allocation_result(&throttle_);
    buffer->data()->reset(openlcb::TractionThrottleCommands::ASSIGN_TRAIN,
                          train,true);
    LOG(ALWAYS,"*** RunATrainFlow::startTrain1(): sending ASSIGN_TRAIN command");
    throttle_.send(buffer);
    return wait_and_call(STATE(trainAssigned));
}

StateFlowBase::Action RunATrainFlow::trainAssigned()
{
    LOG(ALWAYS,"*** RunATrainFlow::trainAssigned(): sending SlowSpeed");
    throttle_.set_speed(openlcb::SpeedType(SlowSpeed));
    LOG(ALWAYS,"*** RunATrainFlow::trainAssigned(): waiting for train to complete trip");
    return wait_and_call(STATE(endTrainRun));
}

StateFlowBase::Action RunATrainFlow::endTrainRun()
{
    LOG(ALWAYS,"*** RunATrainFlow::endTrainRun(): Sending StopSpeed");
    throttle_.set_speed(openlcb::SpeedType(StopSpeed));
    return wait_and_call(STATE(trainStopped));
}

StateFlowBase::Action RunATrainFlow::trainStopped()
{
    LOG(ALWAYS,"*** RunATrainFlow::trainStopped(): allocating buffer to RELEASE_TRAIN");
    return allocate_and_call(&throttle_,STATE(endTrainRun1));
}

StateFlowBase::Action RunATrainFlow::endTrainRun1()
{
    LOG(ALWAYS,"*** RunATrainFlow::endTrainRun1()");
    Buffer<openlcb::TractionThrottleInput> *buffer =
          get_allocation_result(&throttle_);
    buffer->data()->reset(openlcb::TractionThrottleCommands::RELEASE_TRAIN);
    LOG(ALWAYS,"*** RunATrainFlow::endTrainRun1(): sending RELEASE_TRAIN command");
    throttle_.send(buffer);
    LOG(ALWAYS,"*** RunATrainFlow::endTrainRun1(): reseting variables");
    currentTrain->done.notify();
    currentTrain = nullptr;
    currentRoute = nullptr;
    terminal = WendellDepot::NUM_SENSORS;
    currentTurnout = RunATrainFlow::TURNOUT_STATES;
    turnoutIndx_ = WendellDepot::NUM_TURNOUTS;
    desiredState_ = Turnout::UNKNOWN;
    currentSignal = RunATrainFlow::SIGNAL_STATES;
    return release_and_exit();
}
        
        
void RunATrainFlow::turnout_state(WendellDepot::TurnoutIndexes loc, 
                                  Turnout::State_t state,
                                  openlcb::EventReport *event,
                                  BarrierNotifiable *done)
{
    LOG(ALWAYS,"*** RunATrainFlow::turnout_state(%d,%d.%p%p)",loc,state,event,done);
    LOG(ALWAYS,"*** RunATrainFlow::turnout_state(): turnoutIndx_ = %d, desiredState_ = %d",turnoutIndx_,desiredState_);
    if (turnoutIndx_ == loc && state == desiredState_)
    {
        notify();
    }
}

void RunATrainFlow::Covered(WendellDepot::SensorIndexes loc,
                            openlcb::EventReport *event,
                            BarrierNotifiable *done)
{
    auto bps_iter = BlockProtectionSignals.find(loc);
    if (bps_iter != BlockProtectionSignals.end())
    {
        RouteSignalState s = bps_iter->second;
        signals_[(uint)s.signalIndex]->SetAspect(s.signalAspect,done->new_child());
    }
    SpeedKey key(loc,currentDirection,IsCovered);
    auto newspeed_iter = SpeedUpdateMap.find(key);
    if (newspeed_iter != SpeedUpdateMap.end())
    {
        throttle_.set_speed(newspeed_iter->second);
    }
    
}

void RunATrainFlow::Uncovered(WendellDepot::SensorIndexes loc,
                              openlcb::EventReport *event,
                              BarrierNotifiable *done)
{
    SpeedKey key(loc,currentDirection,IsUncovered);
    auto newspeed_iter = SpeedUpdateMap.find(key);
    if (newspeed_iter != SpeedUpdateMap.end())
    {
        throttle_.set_speed(newspeed_iter->second);
    }
    if (loc == terminal)
    {
        notify();
    }
}

