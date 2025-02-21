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
//  Last Modified : <250220.1031>
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
        RunATrainFlow::RouteTurnoutList::Right,
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
        RunATrainFlow::RouteTurnoutList::Right,
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
        RunATrainFlow::RouteTurnoutList::Left,
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
        RunATrainFlow::RouteTurnoutList::Left,
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


RunATrainFlow::RunATrainFlow(Service *service, openlcb::Node *node)
      : RunATrainFlowBase(service)
, node_(node)
, currentTrain(nullptr)
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
    currentTrain = message()->data();
    currentRoute = &routes_[(uint)(currentTrain->route)];
    terminal = currentRoute->terminalLocation;
    currentTurnout = 0;
    bn_.reset(this);
    return call_immediately(STATE(setTurnout));
}

StateFlowBase::Action RunATrainFlow::setTurnout()
{
    if (currentTurnout == TURNOUT_STATES)
    {
        return call_immediately(STATE(startTrain));
    }
    else
    {
        const RouteTurnoutState *ct = &currentRoute->turnoutStates[currentTurnout];
        switch (ct->state)
        {
        case Turnout::NORMAL:
            turnouts_[(uint)(ct->turnoutIndex)]->Normal(bn_.new_child());
            break;
        case Turnout::REVERSE:
            turnouts_[(uint)(ct->turnoutIndex)]->Reverse(bn_.new_child());
            break;
        default:
            break;
        }
        currentSignal = 0;
        return wait_and_call(STATE(setSignal));
    }
}

StateFlowBase::Action RunATrainFlow::setSignal()
{
    if (currentSignal == SIGNAL_STATES)
    {
        currentTurnout++;
        return call_immediately(STATE(setTurnout));
    }
    else
    {
        const RouteTurnoutState *ct = &currentRoute->turnoutStates[currentTurnout];
        const RouteSignalState *cs = &(ct->signalStates[currentSignal]);
        signals_[(uint)(cs->signalIndex)]->SetAspect(cs->signalAspect,bn_.new_child());
        currentSignal++;
        return wait_and_call(STATE(setSignal));
    }
}

StateFlowBase::Action RunATrainFlow::startTrain()
{
    return release_and_exit(); // temp for now
}
        
        
void RunATrainFlow::turnout_state(WendellDepot::TurnoutIndexes loc, 
                                  Turnout::State_t state)
{
    //if (currentTrain == nullptr) return;
    bool needmaybedone = false;
    if (bn_.is_done())
    {
        bn_.reset(this);
        needmaybedone = true;
    }
    switch (loc)
    {
    case WendellDepot::Stage_East_1_3:
        switch (state)
        {
        case Turnout::NORMAL:
            break;
        case Turnout::REVERSE:
            break;
        default: break;
        }
        break;
    case WendellDepot::Stage_East_2_4:
        switch (state)
        {
        case Turnout::NORMAL:
            break;
        case Turnout::REVERSE:
            break;
        default: break;
        }
        break;
    case WendellDepot::Stage_West_2_4:
        switch (state)
        {
        case Turnout::NORMAL:
            break;
        case Turnout::REVERSE:
            break;
        default: break;
        }
        break;
    case WendellDepot::Stage_West_1_3:
        switch (state)
        {
        case Turnout::NORMAL:
            break;
        case Turnout::REVERSE:
            break;
        default: break;
        }
        break;
    case WendellDepot::East_Double_Track:
        switch (state)
        {
        case Turnout::NORMAL:
            break;
        case Turnout::REVERSE:
            break;
        default: break;
        }
        break;
    case WendellDepot::West_Double_Track:
        switch (state)
        {
        case Turnout::NORMAL:
            break;
        case Turnout::REVERSE:
            break;
        default: break;
        }
        break;
    default:
        break;
    }
    if (needmaybedone) bn_.maybe_done();
}

void RunATrainFlow::Covered(WendellDepot::SensorIndexes loc)
{
    // Optical sensor covered. switch over loc and adjust speed and signals, etc.
}

void RunATrainFlow::Uncovered(WendellDepot::SensorIndexes loc)
{
    // Optical sensor uncovered. switch over loc and adjust speed and signals, etc.
}

