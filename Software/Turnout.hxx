// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jan 4 21:19:15 2025
//  Last Modified : <250221.1011>
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
/// @file Turnout.hxx
/// @author Robert Heller
/// @date Sat Jan 4 21:19:15 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __TURNOUT_HXX
#define __TURNOUT_HXX

#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "PointSensor.hxx"
#include "WendellDepot.hxx"

class RunATrainFlow;

class Turnout : public openlcb::SimpleEventHandler
{
public:
    enum State_t {NORMAL, REVERSE, UNKNOWN};
    Turnout(openlcb::Node *node,
            openlcb::EventId normal,
            openlcb::EventId reverse,
            openlcb::EventId pointsnormal,
            openlcb::EventId pointsreverse,
            WendellDepot::TurnoutIndexes loc,
            RunATrainFlow *parent)
                : node_(node)
          , normal_(normal)
          , reverse_(reverse)
          , points_(node,pointsnormal,pointsreverse,this)
          , loc_(loc)
          , parent_(parent)
          , state_(UNKNOWN)
    {
        register_handler();
    }
    ~Turnout()
    {
        unregister_handler();
    }
    void Normal(BarrierNotifiable *done)
    {
        SendEvent(normal_,done);
    }
    void Reverse(BarrierNotifiable *done)
    {
        SendEvent(reverse_,done);
    }
    State_t State() const {return state_;}
    void SetState(State_t newstate,openlcb::EventReport *event,BarrierNotifiable *done);
protected:
    virtual void 
          handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                 openlcb::EventReport *event,
                                 BarrierNotifiable *done);
    virtual void
          handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
                                   openlcb::EventReport *event,
                                   BarrierNotifiable *done);
private:
    void register_handler();
    void unregister_handler();
    void SendEvent(openlcb::EventId event,BarrierNotifiable *done);
    openlcb::Node *node_;
    openlcb::EventId normal_;
    openlcb::EventId reverse_;
    PointSensor points_;
    WendellDepot::TurnoutIndexes loc_;
    RunATrainFlow *parent_;
    State_t state_;
    openlcb::WriteHelper helper_;
};
          

#endif // __TURNOUT_HXX

