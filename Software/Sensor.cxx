// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jan 4 20:14:16 2025
//  Last Modified : <250105.0915>
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
/// @file Sensor.cxx
/// @author Robert Heller
/// @date Sat Jan 4 20:14:16 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include "openlcb/Defs.hxx"
#include "openlcb/WriteHelper.hxx"
#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "Sensor.hxx"

void Sensor::check_sensor(BarrierNotifiable *done)
{
    helpers_[0].WriteAsync(node_,openlcb::Defs::MTI_PRODUCER_IDENTIFY,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(on_),done->new_child());
    helpers_[1].WriteAsync(node_,openlcb::Defs::MTI_PRODUCER_IDENTIFY,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(off_),done->new_child());
    done->maybe_done();
}

void Sensor::handle_event_report(const openlcb::EventRegistryEntry &registry_entry,
                                 openlcb::EventReport *event,
                                 BarrierNotifiable *done)
{
    if (event->event == on_)
    {
        handle_on(done);
    }
    else if (event->event == off_)
    {
        handle_off(done);
    }
    done->maybe_done();
    
}
void Sensor::handle_producer_identified(const openlcb::EventRegistryEntry &registry_entry,
                                        openlcb::EventReport *event, 
                                        BarrierNotifiable *done)
{
    bool value;
    if (event->state == openlcb::EventState::VALID)
    {
        value = true;
    }
    else if (event->state == openlcb::EventState::INVALID)
    {
        value = false;
    }
    else
    {
        done->notify();
        return; // nothing to learn from this message.
    }
    if (event->event == on_ && value)
    {
        handle_on(done);
    }
    else if (event->event == off_ && value)
    {
        handle_off(done);
    }
    else
    {
        done->notify();
        return;
    }
    done->maybe_done();
    return;
              
    
}

void Sensor::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
          openlcb::EventRegistryEntry(this, on_, 0), 0);
    openlcb::EventRegistry::instance()->register_handler(
          openlcb::EventRegistryEntry(this, off_, 0), 0);
}
void Sensor::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

