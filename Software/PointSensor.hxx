// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jan 4 21:13:45 2025
//  Last Modified : <250221.1006>
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
/// @file PointSensor.hxx
/// @author Robert Heller
/// @date Sat Jan 4 21:13:45 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __POINTSENSOR_HXX
#define __POINTSENSOR_HXX

#include "openlcb/EventHandler.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "executor/Notifiable.hxx"
#include "Sensor.hxx"

class Turnout;

class PointSensor : public Sensor
{
public:
    PointSensor(openlcb::Node *node,
                openlcb::EventId on,
                openlcb::EventId off,
                Turnout *parent)
                : Sensor(node,on,off)
          , parent_(parent)
    {
    }
    virtual void handle_on(openlcb::EventReport *event,BarrierNotifiable *done);
    virtual void handle_off(openlcb::EventReport *event,BarrierNotifiable *done);
private:
    Turnout *parent_;
};

#endif // __POINTSENSOR_HXX

