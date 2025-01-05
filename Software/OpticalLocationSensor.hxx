// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jan 4 21:05:33 2025
//  Last Modified : <250104.2113>
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
/// @file OpticalLocationSensor.hxx
/// @author Robert Heller
/// @date Sat Jan 4 21:05:33 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __OPTICALLOCATIONSENSOR_HXX
#define __OPTICALLOCATIONSENSOR_HXX

#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "Sensor.hxx"
#include "WendellDepot.hxx"

class RunATrain;

class OpticalLocationSensor : public Sensor
{
public:
    OpticalLocationSensor(openlcb::Node *node,
                          openlcb::EventId on,
                          openlcb::EventId off,
                          WendellDepot::SensorIndexes loc,
                          RunATrain *parent)
                : Sensor(node,on,off)
          , loc_(loc)
          , parent_(parent)
    {
    }
    virtual void handle_on(BarrierNotifiable *done);
    virtual void handle_off(BarrierNotifiable *done);
private:
    WendellDepot::SensorIndexes loc_;
    RunATrain *parent_;
};
    

#endif // __OPTICALLOCATIONSENSOR_HXX

