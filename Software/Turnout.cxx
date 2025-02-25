// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jan 5 09:18:38 2025
//  Last Modified : <250224.2007>
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
/// @file Turnout.cxx
/// @author Robert Heller
/// @date Sun Jan 5 09:18:38 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "PointSensor.hxx"
#include "Turnout.hxx"
#include "RunATrain.hxx"

void Turnout::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                openlcb::EventReport *event,
                                BarrierNotifiable *done)
{
    if (event->src_node.id == node_->node_id())
    {
        // We don't respond to queries from our own node. This is not nice, but
        // we
        // want to avoid to answering our own Query command.
        done->notify();
        return;
    }
    if (event->dst_node && event->dst_node != node_)
    {
        return done->notify();
    }
    openlcb::EventState state;
    switch (state_)
    {
    case NORMAL:
        state = openlcb::EventState::VALID; break;
    case REVERSE:
        state = openlcb::EventState::INVALID; break;
    case UNKNOWN:
        state = openlcb::EventState::UNKNOWN; break;
    }
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID + state;
    event->event_write_helper<1>()->WriteAsync(node_,mti,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(normal_),
                                               done->new_child());
    mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID + openlcb::invert_event_state(state);
    event->event_write_helper<2>()->WriteAsync(node_,mti,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(reverse_),
                                               done->new_child());
    done->maybe_done();
}
void Turnout::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
                                  openlcb::EventReport *event,
                                  BarrierNotifiable *done)
{
    if (event->src_node.id == node_->node_id())
    {
        // We don't respond to queries from our own node. This is not nice, but
        // we
        // want to avoid to answering our own Query command.
        done->notify();
        return;
    }
    openlcb::EventState active;
    if (event->event == normal_)
    {
        switch (state_)
        {
        case NORMAL:
            active = openlcb::EventState::VALID; break;
        case REVERSE:
            active = openlcb::EventState::INVALID; break;
        case UNKNOWN:
            active = openlcb::EventState::UNKNOWN; break;
        }
    }
    else if (event->event == reverse_)
    {
        switch (state_)
        {
        case NORMAL:
            active = openlcb::EventState::INVALID; break;
        case REVERSE:
            active = openlcb::EventState::VALID; break;
        case UNKNOWN:
            active = openlcb::EventState::UNKNOWN; break;
        }
    }
    else
    {
        done->notify();
        return;
    }
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID + active;
    event->event_write_helper<1>()->WriteAsync(node_,mti,
                                               openlcb::WriteHelper::global(), 
                                               openlcb::eventid_to_buffer(event->event), done);
}
void Turnout::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
          openlcb::EventRegistryEntry(this, normal_, 0), 0);
    openlcb::EventRegistry::instance()->register_handler(
          openlcb::EventRegistryEntry(this, reverse_, 0), 0);
}
void Turnout::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}
void Turnout::SendEvent(openlcb::EventId event,Notifiable *done)
{
    helper_.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                       openlcb::WriteHelper::global(), 
                       openlcb::eventid_to_buffer(event), done);
}

void Turnout::SetState(Turnout::State_t newstate,openlcb::EventReport *event,BarrierNotifiable *done)
{
    state_ = newstate;
    parent_->turnout_state(loc_,state_,event,done);
}
