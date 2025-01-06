// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jan 5 19:54:43 2025
//  Last Modified : <250105.2031>
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
/// @file Signal.cxx
/// @author Robert Heller
/// @date Sun Jan 5 19:54:43 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "WendellDepot.hxx"
#include "Signal.hxx"

void Signal::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                    openlcb::EventReport *event,
                                    BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        return done->notify();
    }
    for (uint i=0; i < WendellDepot::SignalConfig::ASPECTCOUNT; i++)
    {
        if (cfg_.aspects[i].aspect == WendellDepot::SignalConfig::UNUSED)
        {
            break;
        }
        openlcb::EventState state;
        if (cfg_.aspects[i].aspect == currentAspect_)
        {
            state = openlcb::EventState::VALID;
        }
        else
        {
            state = openlcb::EventState::INVALID;
        }
        openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID + state;
        helpers_[i].WriteAsync(node_,mti,openlcb::WriteHelper::global(),
                               openlcb::eventid_to_buffer(cfg_.aspects[i].eventid),
                               done->new_child());
    }
    done->maybe_done();
}
void Signal::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
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
    for (uint i=0; i < WendellDepot::SignalConfig::ASPECTCOUNT; i++)
    {
        if (cfg_.aspects[i].eventid == event->event)
        {
            if (cfg_.aspects[i].aspect == currentAspect_)
            {
                active = openlcb::EventState::VALID;
            }
            else
            {
                active = openlcb::EventState::INVALID;
            }
            openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID + active;
            event->event_write_helper<1>()->WriteAsync(node_,mti,
                                                       openlcb::WriteHelper::global(), 
                                                       openlcb::eventid_to_buffer(event->event), done);
            return;
        }
    }
    done->notify();
    return;
}
void Signal::register_handler()
{
    for (uint i=0; i < WendellDepot::SignalConfig::ASPECTCOUNT; i++)
    {
        if (cfg_.aspects[i].aspect == WendellDepot::SignalConfig::UNUSED)
        {
            break;
        }
        openlcb::EventRegistry::instance()->register_handler(
             openlcb::EventRegistryEntry(this, cfg_.aspects[i].eventid, 0), 0);
    }
}
void Signal::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}
void Signal::SendEvent(openlcb::EventId event,BarrierNotifiable *done)
{
    helpers_[0].WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                          openlcb::WriteHelper::global(),
                          openlcb::eventid_to_buffer(event), done);
}

