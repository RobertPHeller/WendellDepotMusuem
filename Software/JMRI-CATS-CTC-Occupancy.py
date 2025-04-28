#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Apr 28 08:07:45 2025
#  Last Modified : <250428.1345>
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

#** Class that uses optical sensors to implement block occupancy.

class opticalSensorToBlockOccupancy(jmri.jmrit.automat.AbstractAutomaton):
    #** Sensor to Signal Mast map.  This dictionary maps optical sensors to
    #* Signal Mast.  The Signal Mast aspect is either Stop, which implies the
    #* direction is away from the Signal Mast (the mast is facing against the 
    #* train direction), or the aspect is not Stop, which implies the mast
    #* facing the front of the train.
    sensorSignalMastMap = { \
        "West Stage Exit 1" : ("WestExit1 (Dwarf)", ("East Stage Exit 1", "Stage_1"), \
                                                    ("Stage West 1-3 Entrance", "OS Stage_West_1_3") ),\
        "West Stage Exit 3" : ("WestExit3 (Dwarf)", ("East Stage Exit 3", "Stage_3"), \
                                                    ("Stage West 1-3 Entrance", "OS Stage_West_1_3") ),\
        "Stage West 1-3 Entrance" : ("WestYardEntry1-3", ("West Double Track Frog Stop",  "WestMainTrack1"), \
                                    (["West Stage Exit 1", "West Stage Exit 3"], "OS Stage_West_1_3") ), \
        "West Stage Exit 2" : ("WestExit2 (Dwarf)", ("East Stage Exit 2", "Stage_2"), \
                                                    ("Stage West 2-4 Entrance",  "OS Stage_West_2_4") ),\
        "West Stage Exit 4" : ("WestExit4 (Dwarf)", ("East Stage Exit 4", "Stage_4"),\
                                                    ("Stage West 2-4 Entrance", "OS Stage_West_2_4") ), \
        "Stage West 2-4 Entrance" : ("WestYardEntry2-4", ("West Double Track Straight Stop", "WestMainTrack2"), \
                                                         (["West Stage Exit 2", "West Stage Exit 4"], "OS Stage_West_2_4") ), \
        "West Double Track Frog Stop" : ("WestF1", ("Stage West 1-3 Entrance", "WestMainTrack1"),\
                                                   ("West Double Track Entrance", "OS West_Double_Track") ),\
        "West Double Track Straight Stop" : ("WestFM", ("Stage West 2-4 Entrance", "WestMainTrack2"), \
                                                       ("West Double Track Entrance", "OS West_Double_Track") ),\
        "West Double Track Entrance" : ("WestEN", ("East Double Track Entrance", "WendellDepot"), \
                                                  (["West Double Track Frog Stop", "West Double Track Straight Stop"], "OS West Double Track") ), \
        "East Double Track Entrance" : ("EastEN", ("West Double Track Entrance", "WendellDepot"),\
                                                  (["East Double Track Frog Stop", "East Double Track Straight Stop"], "OS East_Double_Track") ),\
        "East Double Track Frog Stop" : ("EastF1", ("Stage East 2-4 Entrance",  "EastMainTrack2"), \
                                                  ("East Double Track Entrance", "OS East_Double_Track") ),\
        "East Double Track Straight Stop" : ("EastFM" , ("Stage East 1-3 Entrance", "EastMainTrack1"), \
                                            ("East Double Track Entrance", "OS East_Double_Track") ),\
        "Stage East 1-3 Entrance" : ("EastYardEntry1-3", ("East Double Track Straight Stop", "EastMainTrack1"),\
                                                         (["East Stage Exit 1", "East Stage Exit 3"], "OS Stage_East_1_3") ),\
        "East Stage Exit 1" : ("EastExit1 (Dwarf)", ("West Stage Exit 1", "Stage_1"), \
                                                    ("Stage East 1-3 Entrance", "OS Stage_East_1_3") ),\
        "East Stage Exit 3" : ("EastExit3 (Dwarf)", ("West Stage Exit 3", "Stage_3"),\
                                                    ("Stage East 1-3 Entrance", "OS Stage_East_1_3") ), \
        "Stage East 2-4 Entrance" : ("EastYardEntry2-4", ("East Double Track Frog Stop", "EastMainTrack2"),\
                                                         (["East Stage Exit 2", "East Stage Exit 4"], "OS Stage_East_2_4") ),\
        "East Stage Exit 2" : ("EastExit2 (Dwarf)", ("West Stage Exit 2", "Stage_2"), \
                                                    ("Stage East 2-4 Entrance", "OS Stage_East_2_4") ),\
        "East Stage Exit 4" : ("EastExit4 (Dwarf)", ("West Stage Exit 4", "Stage_4"), \
                                                    ("Stage East 2-4 Entrance", "OS Stage_East_2_4") ),\
    }
    #** This dictionary maps signals that need to fall down (change to Stop)
    #* when a block is entered.
    blockProtectionSignals = { \
        "OS Stage_West_1_3": ["WestYardEntry1-3", "WestExit1 (Dwarf)", "WestExit3 (Dwarf)"], \
        "OS Stage_East_1_3": ["EastYardEntry1-3", "EastExit1 (Dwarf)", "EastExit3 (Dwarf)"], \
        "OS Stage_West_2_4": ["WestYardEntry2-4", "WestExit2 (Dwarf)", "WestExit4 (Dwarf)"], \
        "OS Stage_East_2_4": ["EastYardEntry2-4", "EastExit2 (Dwarf)", "EastExit4 (Dwarf)"], \
        "OS West_Double_Track": ["WestEN", "WestF1", "WestFM"], \
        "OS East_Double_Track": ["EastEN", "EastF1", "EastFM"] \
    }
    #** Initialize things: build a list of NameBeans (optical sensors) to wait
    #* on, stash their initial state, and create an empty dictionary for 
    #* run time state information.
    def init(self):
        self.opticalSensors = list()
        self.sensorKnownStates = dict()
        self.sensorStates = dict()
        for key in self.sensorSignalMastMap:
            #print key
            sensor = sensors.getSensor(key)
            #print sensor
            self.opticalSensors.append(sensor)
            self.sensorKnownStates[key] = sensor.getKnownState()
    #** Running handler: wait for a sensor to trip, then process the event.
    def handle(self):
        #* Wait...
        self.waitChange(self.opticalSensors)
        #* find the changed sensor(s)
        for s in self.opticalSensors:
            #* Get name of the sensor
            name = s.getUserName()
            #print name,":",s
            #print self.sensorKnownStates[name], s.getKnownState()
            #* Has its state changed?
            if self.sensorKnownStates[name] != s.getKnownState():
                #print name, s.getKnownState()
                #* update stored state
                self.sensorKnownStates[name] = s.getKnownState()
                #* Sensor covered?
                if s.getKnownState() == ACTIVE:
                    #* yes, get signal mast, ifstop, ifclear
                    mast, ifstop, ifclear = self.sensorSignalMastMap[name]
                    otherends, blockname = (None, None)
                    if masts.getSignalMast(mast).getAspect() == "Stop":
                        otherends, blockname = ifstop
                    else:
                        otherends, blockname = ifclear
                    if otherends != None and blockname != None:
                        #* get the actual block
                        block = blocks.getBlock(blockname)
                        #print "Entering:",blockname,block
                        #* Set the block occupied
                        block.getSensor().setKnownState(ACTIVE)
                        #* Are there signals that need to fall?
                        if self.blockProtectionSignals.has_key(blockname):
                            #* Yes, drop them (set their aspects to Stop)
                            for sig in self.blockProtectionSignals[blockname]:
                                mast = masts.getSignalMast(sig)
                                mast.setAspect("Stop")
                        #* Stash the other ends
                        if type(otherends) is list:
                            for o in otherends:
                                self.sensorStates[o] = blockname
                        else:
                            self.sensorStates[otherends] = blockname
                else:
                    #* Sensor cleared, unoccupy the block, if we are waiting
                    if self.sensorStates.has_key(name):
                        blockname = self.sensorStates[name]
                        del self.sensorStates[name]
                        block = blocks.getBlock(blockname)
                        #print "Exiting:",blockname,block
                        block.getSensor().setKnownState(INACTIVE)
            #break
        #* Wait for more
        return True

#* Create an instance
o = opticalSensorToBlockOccupancy();

#* Set its name
o.setName("Optical Sensor To Block Occupancy")

#* start it running
o.start()
                        

