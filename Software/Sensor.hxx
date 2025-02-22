// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jan 4 19:53:25 2025
//  Last Modified : <250221.1005>
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
/// @file Sensor.hxx
/// @author Robert Heller
/// @date Sat Jan 4 19:53:25 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __SENSOR_HXX
#define __SENSOR_HXX

#include "openlcb/WriteHelper.hxx"
#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"

class Sensor : public openlcb::SimpleEventHandler
{
public:
    Sensor(openlcb::Node *node,
           openlcb::EventId on,
           openlcb::EventId off)
                : node_(node)
          , on_(on)
          , off_(off)
          , init_(false)
    {
        register_handler();
    }
    virtual ~Sensor()
    {
        unregister_handler();
    }
    virtual void handle_on(openlcb::EventReport *event,BarrierNotifiable *done) = 0;
    virtual void handle_off(openlcb::EventReport *event,BarrierNotifiable *done) = 0;
protected:
    virtual void handle_event_report(const openlcb::EventRegistryEntry &registry_entry,
                                     openlcb::EventReport *event,
                                     BarrierNotifiable *done);
    virtual void handle_producer_identified(const openlcb::EventRegistryEntry &registry_entry,
                                            openlcb::EventReport *event, 
                                            BarrierNotifiable *done);
    virtual void 
          handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                 openlcb::EventReport *event,
                                 BarrierNotifiable *done);
    virtual void 
          handle_identify_consumer(const openlcb::EventRegistryEntry &registry_entry,
                                 openlcb::EventReport *event,
                                 BarrierNotifiable *done);
    void SendConsumerIdentified(openlcb::EventReport *event, BarrierNotifiable *done);
private:
    void register_handler();
    void unregister_handler();
    openlcb::Node *node_;
    openlcb::EventId on_;
    openlcb::EventId off_;
    bool init_;
    openlcb::WriteHelper helpers_[2];
};


#endif // __SENSOR_HXX

