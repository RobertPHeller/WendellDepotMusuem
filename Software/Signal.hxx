// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jan 5 19:42:44 2025
//  Last Modified : <250224.2008>
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
/// @file Signal.hxx
/// @author Robert Heller
/// @date Sun Jan 5 19:42:44 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __SIGNAL_HXX
#define __SIGNAL_HXX

#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "WendellDepot.hxx"

class Signal : public openlcb::SimpleEventHandler
{
public:
    Signal(openlcb::Node *node,
           const WendellDepot::SignalConfig &cfg)
                : node_(node)
          , cfg_(cfg)
          , currentAspect_(WendellDepot::SignalConfig::UNUSED)
    {
        register_handler();
    }
        
    ~Signal()
    {
        unregister_handler();
    }
    void SetAspect(WendellDepot::SignalConfig::Aspect aspect,
                   Notifiable *done)
    {
        for (uint i=0; i < WendellDepot::SignalConfig::ASPECTCOUNT; i++)
        {
            if (cfg_.aspects[i].aspect == aspect)
            {
                SendEvent(cfg_.aspects[i].eventid, done);
                currentAspect_ = aspect;
                break;
            }
        }
    }
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
    void SendEvent(openlcb::EventId eventid,Notifiable *done); 
    openlcb::Node *node_;
    const WendellDepot::SignalConfig cfg_;
    WendellDepot::SignalConfig::Aspect currentAspect_;
    openlcb::WriteHelper helpers_[WendellDepot::SignalConfig::ASPECTCOUNT+1];
};
           

#endif // __SIGNAL_HXX

