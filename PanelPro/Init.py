#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri May 16 10:25:57 2025
#  Last Modified : <250516.1049>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2025  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


import jarray
import jmri

Clevers = ["Signal Lever Stage West 1-3C", "Signal Lever Stage West 2-4C",
           "Signal Lever West DoubleC", "Signal Lever East DoubleC",
           "Signal Lever Stage East 1-3C", "Signal Lever Stage East 2-4C"]
Clamps = ["Signal Lamp Stage West 1-3C", "Signal Lamp Stage West 2-4C",
          "Signal Lamp West DoubleC", "Signal Lamp East DoubleC",
          "Signal Lamp Stage East 1-3C", "Signal Lamp Stage East 2-4C"]
          
for lev in Clevers:
    print(lev)
    sensors.getSensor(lev).setKnownState(ACTIVE)
    
for lamp in Clamps:
    print(lamp)
    sensors.getSensor(lamp).setKnownState(ACTIVE)

Signals = ["EastExit4 (Dwarf)", "EastExit2 (Dwarf)", "EastExit1 (Dwarf)", 
           "EastExit3 (Dwarf)", "WestExit3 (Dwarf)", "WestExit1 (Dwarf)", 
           "WestExit2 (Dwarf)", "WestExit4 (Dwarf)", "WestF1", "WestFM", 
           "EastF1", "EastFM", "WestEN", "WestYardEntry1-3", 
           "WestYardEntry2-4", "EastYardEntry1-3", "EastYardEntry2-4", 
           "EastEN"]

for sig in Signals:
    print(sig)
    mast = masts.getSignalMast(sig)
    mast.setAspect("Stop")
