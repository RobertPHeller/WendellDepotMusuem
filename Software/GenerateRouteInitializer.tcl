#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Tue Feb 11 22:22:40 2025
#  Last Modified : <250213.1555>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
## @copyright
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
# @file GenerateRouteInitializer.tcl
# @author Robert Heller
# @date Tue Feb 11 22:22:40 2025
# 
#
#*****************************************************************************


package require snit

snit::type RouteSignalState {
    variable signalIndex
    variable signalAspect
    constructor {i a args} {
        set signalIndex $i
        set signalAspect $a
    }
    method GenerateInitializer { {fp stdout} {indent {    }} {comma {}}} {
        puts $fp "${indent}\{"
        puts $fp "${indent}    ${signalIndex},"
        puts $fp "${indent}    ${signalAspect}"
        puts $fp "${indent}\}$comma"
    }
}


snit::type RouteTurnoutState {
    variable turnoutIndex
    variable state
    variable signalStates
    constructor {i s ss_list args} {
        set turnoutIndex $i
        set state $s
        set signalStates $ss_list
    }
    method GenerateInitializer { {fp stdout} {indent {    }} {comma {}}} {
        puts $fp "${indent}\{"
        puts $fp "${indent}    ${turnoutIndex},"
        puts $fp "${indent}    ${state},"
        puts $fp "${indent}    \{"
        foreach ss $signalStates {
            if {$ss eq [lindex $signalStates end]} {
                $ss GenerateInitializer $fp "${indent}        "
            } else {
                $ss GenerateInitializer $fp "${indent}        " {,}
            }
        }
        puts $fp "${indent}    \}"
        puts $fp "${indent}\}${comma}"
    }
}


snit::type RouteTurnoutList {
    variable exitLocation
    variable turnoutStates
    constructor {exit ts_list args} {
        set exitLocation $exit
        set turnoutStates $ts_list
    }
    method GenerateInitializer {{fp stdout} {indent {    }} {comma {}}} {
        puts $fp "${indent}\{"
        puts $fp "${indent}    ${exitLocation},"
        puts $fp "${indent}    \{"
        foreach ts $turnoutStates {
            if {$ts eq [lindex $turnoutStates end]} {
                $ts GenerateInitializer $fp "${indent}        "
            } else {
                $ts GenerateInitializer $fp "${indent}        " {,}
            }
        }
        puts $fp "${indent}    \}"
        puts $fp "${indent}\}${comma}"
    }
}

set routes_ [list]

lappend routes_ [RouteTurnoutList %AUTO% \
                 WendellDepot::West_Stage_Exit_3 \
                 [list [RouteTurnoutState %AUTO% WendellDepot::Stage_West_1_3 \
                       Turnout::REVERSE \
                       [list [RouteSignalState %AUTO% WendellDepot::WestYardEntry1_3 WendellDepot::SignalConfig::Stop] \
                        [RouteSignalState %AUTO% WendellDepot::WestExit1__Dwarf_ WendellDepot::SignalConfig::Hold] \
                        [RouteSignalState %AUTO% WendellDepot::WestExit3__Dwarf_ WendellDepot::SignalConfig::Go]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::West_Double_Track \
                        Turnout::REVERSE \
                        [list [RouteSignalState %AUTO% WendellDepot::WestEN WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::WestF1 WendellDepot::SignalConfig::Slow_Clear] \
                         [RouteSignalState %AUTO% WendellDepot::WestFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::East_Double_Track \
                        Turnout::NORMAL \
                        [list [RouteSignalState %AUTO% WendellDepot::EastEN WendellDepot::SignalConfig::Clear] \
                         [RouteSignalState %AUTO% WendellDepot::EastF1 WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::EastFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::Stage_East_1_3 \
                        Turnout::REVERSE \
                        [list [RouteSignalState %AUTO% WendellDepot::EastYardEntry1_3 WendellDepot::SignalConfig::Slow_Clear] \
                         [RouteSignalState %AUTO% WendellDepot::EastExit1__Dwarf_ WendellDepot::SignalConfig::Hold] \
                         [RouteSignalState %AUTO% WendellDepot::EastExit3__Dwarf_ WendellDepot::SignalConfig::Hold]]]]]

#puts stderr $routes_

lappend routes_ [RouteTurnoutList %AUTO% \
                 WendellDepot::West_Stage_Exit_1 \
                 [list [RouteTurnoutState %AUTO% WendellDepot::Stage_West_1_3 \
                       Turnout::NORMAL \
                       [list [RouteSignalState %AUTO% WendellDepot::WestYardEntry1_3 WendellDepot::SignalConfig::Stop] \
                        [RouteSignalState %AUTO% WendellDepot::WestExit1__Dwarf_ WendellDepot::SignalConfig::Go] \
                        [RouteSignalState %AUTO% WendellDepot::WestExit3__Dwarf_ WendellDepot::SignalConfig::Hold]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::West_Double_Track \
                        Turnout::REVERSE \
                        [list [RouteSignalState %AUTO% WendellDepot::WestEN WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::WestF1 WendellDepot::SignalConfig::Slow_Clear] \
                         [RouteSignalState %AUTO% WendellDepot::WestFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::East_Double_Track \
                        Turnout::NORMAL \
                        [list [RouteSignalState %AUTO% WendellDepot::EastEN WendellDepot::SignalConfig::Clear] \
                         [RouteSignalState %AUTO% WendellDepot::EastF1 WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::EastFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::Stage_East_1_3 \
                        Turnout::NORMAL \
                        [list [RouteSignalState %AUTO% WendellDepot::EastYardEntry1_3 WendellDepot::SignalConfig::Clear] \
                         [RouteSignalState %AUTO% WendellDepot::EastExit1__Dwarf_ WendellDepot::SignalConfig::Hold] \
                         [RouteSignalState %AUTO% WendellDepot::EastExit3__Dwarf_ WendellDepot::SignalConfig::Hold]]]]]

#puts stderr $routes_

lappend routes_ [RouteTurnoutList %AUTO% \
                 WendellDepot::West_Stage_Exit_4 \
                 [list [RouteTurnoutState %AUTO% WendellDepot::Stage_East_2_4 \
                       Turnout::REVERSE \
                       [list [RouteSignalState %AUTO% WendellDepot::EastYardEntry2_4 WendellDepot::SignalConfig::Stop] \
                        [RouteSignalState %AUTO% WendellDepot::EastExit2__Dwarf_ WendellDepot::SignalConfig::Hold] \
                        [RouteSignalState %AUTO% WendellDepot::EastExit4__Dwarf_ WendellDepot::SignalConfig::Go]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::East_Double_Track \
                        Turnout::REVERSE \
                        [list [RouteSignalState %AUTO% WendellDepot::EastEN WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::EastF1 WendellDepot::SignalConfig::Slow_Clear] \
                         [RouteSignalState %AUTO% WendellDepot::EastFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::West_Double_Track \
                        Turnout::NORMAL \
                        [list [RouteSignalState %AUTO% WendellDepot::WestEN WendellDepot::SignalConfig::Clear] \
                         [RouteSignalState %AUTO% WendellDepot::WestF1 WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::WestFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::Stage_West_2_4 \
                        Turnout::REVERSE \
                        [list [RouteSignalState %AUTO% WendellDepot::WestYardEntry2_4 WendellDepot::SignalConfig::Slow_Clear] \
                         [RouteSignalState %AUTO% WendellDepot::WestExit2__Dwarf_ WendellDepot::SignalConfig::Hold] \
                         [RouteSignalState %AUTO% WendellDepot::WestExit4__Dwarf_ WendellDepot::SignalConfig::Hold]]]]]

#puts stderr $routes_

lappend routes_ [RouteTurnoutList %AUTO% \
                 WendellDepot::West_Stage_Exit_2 \
                 [list [RouteTurnoutState %AUTO% WendellDepot::Stage_East_2_4 \
                       Turnout::NORMAL \
                       [list [RouteSignalState %AUTO% WendellDepot::EastYardEntry2_4 WendellDepot::SignalConfig::Stop] \
                        [RouteSignalState %AUTO% WendellDepot::EastExit2__Dwarf_ WendellDepot::SignalConfig::Go] \
                        [RouteSignalState %AUTO% WendellDepot::EastExit4__Dwarf_ WendellDepot::SignalConfig::Hold]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::East_Double_Track \
                        Turnout::REVERSE \
                        [list [RouteSignalState %AUTO% WendellDepot::EastEN WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::EastF1 WendellDepot::SignalConfig::Slow_Clear] \
                         [RouteSignalState %AUTO% WendellDepot::EastFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::West_Double_Track \
                        Turnout::NORMAL \
                        [list [RouteSignalState %AUTO% WendellDepot::WestEN WendellDepot::SignalConfig::Clear] \
                         [RouteSignalState %AUTO% WendellDepot::WestF1 WendellDepot::SignalConfig::Stop] \
                         [RouteSignalState %AUTO% WendellDepot::WestFM WendellDepot::SignalConfig::Stop]]] \
                       [RouteTurnoutState %AUTO% WendellDepot::Stage_West_2_4 \
                        Turnout::NORMAL \
                        [list [RouteSignalState %AUTO% WendellDepot::WestYardEntry2_4 WendellDepot::SignalConfig::Clear] \
                         [RouteSignalState %AUTO% WendellDepot::WestExit2__Dwarf_ WendellDepot::SignalConfig::Hold] \
                         [RouteSignalState %AUTO% WendellDepot::WestExit4__Dwarf_ WendellDepot::SignalConfig::Hold]]]]]

#puts stderr $routes_

puts "\{"
foreach r $routes_ {
    #puts stderr "$r"
    if {$r eq [ lindex $routes_ end]} {
        $r GenerateInitializer stdout "    "
    } else {
        $r GenerateInitializer stdout "    " {,}
    }
}
puts "\};"

