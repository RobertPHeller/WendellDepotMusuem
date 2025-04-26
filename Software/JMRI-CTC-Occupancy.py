#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sat Apr 26 10:20:35 2025
#  Last Modified : <250426.1627>
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

class opticalSensorToBlockOccupancy(jmri.jmrit.automat.AbstractAutomaton):
    sensorSigPlateMap = { \
            "West Stage Exit 1" : {"Signal Lamp Stage West 1-3L": ("East Stage Exit 1", "Stage_1"), \
                                   "Signal Lamp Stage West 1-3R": ("Stage West 1-3 Entrance", "OS Stage_West_1_3")}, \
            "West Stage Exit 3" : {"Signal Lamp Stage West 1-3L": ("East Stage Exit 3", "Stage_3"), \
                                   "Signal Lamp Stage West 1-3R": ("Stage West 1-3 Entrance", "OS Stage_West_1_3")},\
            "Stage West 1-3 Entrance" : {"Signal Lamp Stage West 1-3L": (["West Stage Exit 1", "West Stage Exit 3"], "OS Stage_West_1_3"), \
                                         "Signal Lamp Stage West 1-3R": ("West Double Track Frog Stop",  "WestMainTrack1")}, \
            "West Stage Exit 2" :  { "Signal Lamp Stage West 2-4L": ("East Stage Exit 2", "Stage_2"),
                                     "Signal Lamp Stage West 2-4R" :("Stage West 2-4 Entrance",  "OS Stage_West_2_4") },\
            "West Stage Exit 4" : {"Signal Lamp Stage West 2-4L": ("East Stage Exit 4", "Stage_4"),\
                                   "Signal Lamp Stage West 2-4R": ("Stage West 2-4 Entrance", "OS Stage_West_2_4")}, \
            "Stage West 2-4 Entrance" : { "Signal Lamp Stage West 2-4L" : (["West Stage Exit 2", "West Stage Exit 4"], "OS Stage_West_2_4"), \
                                          "Signal Lamp Stage West 2-4R" : ("West Double Track Straight Stop", "WestMainTrack2") },\
            "West Double Track Frog Stop" : { "Signal Lamp West DoubleL": ("Stage West 1-3 Entrance", "WestMainTrack1"),\
                                              "Signal Lamp West DoubleR": ("West Double Track Entrance", "OS West_Double_Track") },\
            "West Double Track Straight Stop" : {"Signal Lamp West DoubleL": ("Stage West 2-4 Entrance", "WestMainTrack2"), \
                                                 "Signal Lamp West DoubleR": ("West Double Track Entrance" "OS West_Double_Track")},\
            "West Double Track Entrance" : { "Signal Lamp West DoubleL": (["West Double Track Frog Stop", "West Double Track Straight Stop"], "OS West Double Track"),\
                                             "Signal Lamp West DoubleR": ("East Double Track Entrance", "WendellDepot") },\
            "East Double Track Entrance" : { "Signal Lamp East DoubleL": ("West Double Track Entrance", "WendellDepot"),\
                                             "Signal Lamp East DoubleR": (["East Double Track Frog Stop", "East Double Track Straight Stop"], "OS East_Double_Track")},\
            "East Double Track Frog Stop" : { "Signal Lamp East DoubleL": ("East Double Track Entrance", "OS East_Double_Track"),\
                                            "Signal Lamp East DoubleR": ("Stage East 2-4 Entrance",  "EastMainTrack2")},\
            "East Double Track Straight Stop" : {"Signal Lamp East DoubleL": ("East Double Track Entrance", "OS East_Double_Track"),\
                                                 "Signal Lamp East DoubleR": ("Stage East 1-3 Entrance", "EastMainTrack1")},\
            "Stage East 1-3 Entrance" : {"Signal Lamp Stage East 1-3L": ("East Double Track Straight Stop", "EastMainTrack1"),\
                                         "Signal Lamp Stage East 1-3R": (["East Stage Exit 1", "East Stage Exit 3"], "OS Stage_East_1_3")},\
            "East Stage Exit 1" : {"Signal Lamp Stage East 1-3L":  ("Stage East 1-3 Entrance", "OS Stage_East_1_3"),\
                                           "Signal Lamp Stage East 1-3R": ("West Stage Exit 1", "Stage_1")},\
            "East Stage Exit 3" : {"Signal Lamp Stage East 1-3L":  ("Stage East 1-3 Entrance", "OS Stage_East_1_3"), \
                                   "Signal Lamp Stage East 1-3R": ("West Stage Exit 3", "Stage_3")},\
            "Stage East 2-4 Entrance" : {"Signal Lamp Stage East 2-4L": ("East Double Track Frog Stop", "EastMainTrack2"),\
                                         "Signal Lamp Stage East 2-4R": (["East Stage Exit 2", "East Stage Exit 4"], "OS Stage_East_2_4")},\
            "East Stage Exit 2" : {"Signal Lamp Stage East 2-4L": ("Stage East 2-4 Entrance", "OS Stage_East_2_4"),\
                                   "Signal Lamp Stage East 2-4R": ("West Stage Exit 2", "Stage_2")},\
            "East Stage Exit 4" : {"Signal Lamp Stage East 2-4L": ("Stage East 2-4 Entrance", "OS Stage_East_2_4"),\
                                   "Signal Lamp Stage East 2-4R": ("West Stage Exit 4", "Stage_4")} \
    }
    blockProtectionSignals = { \
        "OS Stage_West_1_3": ["WestYardEntry1-3", "WestExit1 (Dwarf)", "WestExit3 (Dwarf)"], \
        "OS Stage_East_1_3": ["EastYardEntry1-3", "EastExit1 (Dwarf)", "EastExit3 (Dwarf)"], \
        "OS Stage_West_2_4": ["WestYardEntry2-4", "WestExit2 (Dwarf)", "WestExit4 (Dwarf)"], \
        "OS Stage_East_2_4": ["EastYardEntry2-4", "EastExit2 (Dwarf)", "EastExit4 (Dwarf)"], \
        "OS West_Double_Track": ["WestEN", "WestF1", "WestFM"], \
        "OS East_Double_Track": ["EastEN", "EastF1", "EastFM"] \
    }
    def init(self):
        self.opticalSensors = list()
        self.sensorKnownStates = dict()
        self.sensorStates = dict()
        for key in self.sensorSigPlateMap:
            print key
            sensor = sensors.getSensor(key)
            print sensor
            self.opticalSensors.append(sensor)
            self.sensorKnownStates[key] = sensor.getKnownState()
    def handle(self):
        self.waitChange(self.opticalSensors)
        for s in self.opticalSensors:
            name = s.getUserName()
            print name,":",s
            print self.sensorKnownStates[name], s.getKnownState()
            if self.sensorKnownStates[name] != s.getKnownState():
                print name, s.getKnownState()
                self.sensorKnownStates[name] = s.getKnownState()
                if s.getKnownState() == ACTIVE:
                    for k in self.sensorSigPlateMap[name]:
                        if sensors.getSensor(k).getKnownState() == ACTIVE:
                            otherends, blocklist = self.sensorSigPlateMap[name][k]
                            if type(blocklist) is list:
                                for b in blocklist:
                                    block = blocks.getBlock(b)
                                    print "Entering:",b,block
                                    block.getSensor().setKnownState(ACTIVE)
                                    if self.blockProtectionSignals.has_key(b):
                                        for sig in self.blockProtectionSignals[b]:
                                            mast = masts.getSignalMast(sig)
                                            mast.setAspect("Stop")
                            else:
                                block = blocks.getBlock(blocklist)
                                print "Entering:",blocklist,block
                                block.getSensor().setKnownState(ACTIVE)
                                if self.blockProtectionSignals.has_key(blocklist):
                                    for sig in self.blockProtectionSignals[blocklist]:
                                        mast = masts.getSignalMast(sig)
                                        mast.setAspect("Stop")
                            if type(otherends) is list:
                                for o in otherends:
                                    self.sensorStates[o] = blocklist
                            else:
                                self.sensorStates[otherends] = blocklist
                else:
                    if self.sensorStates.has_key(name):
                        blocklist = self.sensorStates[name]
                        del self.sensorStates[name]
                        if type(blocklist) is list:
                            for b in blocklist:
                                block = blocks.getBlock(b)
                                print "Exiting:",b,block
                                block.getSensor().setKnownState(INACTIVE)
                        else:
                            block = blocks.getBlock(blocklist)
                            print "Exiting:",blocklist,block
                            block.getSensor().setKnownState(INACTIVE)
                break
        return True

o = opticalSensorToBlockOccupancy();

o.setName("Optical Sensor To Block Occupancy")

o.start()
