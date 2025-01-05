#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Jan 3 15:41:13 2025
#  Last Modified : <250104.1650>
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
# @file LayoutControlDB2ConfigureCXX.tcl
# @author Robert Heller
# @date Fri Jan 3 15:41:13 2025
# 
#
#*****************************************************************************


lappend auto_path /usr/local/lib/MRRSystem;# C++ (binary) packages
lappend auto_path /usr/local/share/MRRSystem;# Tcl (source) packages
package require snit
package require LayoutControlDB 2.0
package require LCC

set db [lcc::LayoutControlDB olddb ../../../WendellDepot.xml]

set HXX [open "WendellDepot.hxx" w]
set CXX [open "WendellDepot.cxx" w]

puts $HXX "#ifndef WENDELLDEPOT_HXX"
puts $HXX "#define WENDELLDEPOT_HXX 1"
puts $HXX {#include "openlcb/EventHandler.hxx"}

puts $CXX {#include "WendellDepot.hxx"}
puts $CXX {#include "openlcb/EventHandler.hxx"}
    
puts $HXX "namespace WendellDepot \{"
puts $CXX "namespace WendellDepot \{"

proc Name2Symbol {name} {
    return [regsub -all {[[:space:]()-]} $name "_"]
}

proc EventID2LongLongHEX {eventid} {
    if {$eventid eq {}} {
        set event [lcc::EventID %AUTO% -eventidstring {00.00.00.00.00.00.00.00}]
    } else {
        set event [lcc::EventID %AUTO% -eventidstring $eventid]
    }
    set result {}
    foreach b [$event cget -eventidlist] {
        append result [format {%02X} $b]
    }
    return [format "%sLL" $result]
}

puts $HXX "enum SensorIndexes"
puts $HXX "\{"
foreach s [$db getAllSensors] {
    set nt [$s getElementsByTagName name -depth 1]
    puts $HXX [format "\t%s," [Name2Symbol [$nt data]]]
}
puts $HXX "\tNUM_SENSORS"
puts $HXX "\};"
puts $HXX "struct SensorConfig \{ openlcb::EventId on, off; \};"
puts $HXX "extern const SensorConfig SensorsCfg\[NUM_SENSORS\];"
puts $HXX "extern const char *SensorNames\[NUM_SENSORS\];"

puts $CXX "const SensorConfig SensorsCfg\[NUM_SENSORS\] = \{"
foreach s [$db getAllSensors] {
    puts -nonewline $CXX "\t\{"
    puts -nonewline $CXX [EventID2LongLongHEX [[$s getElementsByTagName on -depth 1] data]]
    puts -nonewline $CXX {, }
    puts -nonewline $CXX [EventID2LongLongHEX [[$s getElementsByTagName off -depth 1] data]]
    puts -nonewline $CXX "\},"
    puts $CXX [format {// %s} [[$s getElementsByTagName name -depth 1] data]]
}

puts $CXX "\};"
puts $CXX "const char *SensorNames\[NUM_SENSORS\] = \{" 
foreach s [$db getAllSensors] {
    puts $CXX [format "\t\"%s\"," [[$s getElementsByTagName name -depth 1] data]]
}
puts $CXX "\};"

puts $HXX "enum TurnoutIndexes"
puts $HXX "\{"
foreach t [$db getAllTurnouts] {
    set nt [$t getElementsByTagName name -depth 1]
    puts $HXX [format "\t%s," [Name2Symbol [$nt data]]]
}
puts $HXX "\tNUM_TURNOUTS"
puts $HXX "\};"
puts $HXX "struct TurnoutConfig \{"
puts $HXX "\tstruct \{openlcb::EventId normal,reverse;\} motor;"
puts $HXX "\tstruct \{openlcb::EventId normal,reverse;\} points;"
puts $HXX "\};"
puts $HXX "extern const TurnoutConfig TurnoutsCfg\[NUM_TURNOUTS\];"
puts $HXX "extern const char *TurnoutNames\[NUM_TURNOUTS\];"

puts $CXX "const TurnoutConfig TurnoutsCfg\[NUM_TURNOUTS\] = \{"
foreach t [$db getAllTurnouts] {
    puts $CXX "\t\{"
    set m [$t getElementsByTagName motor -depth 1]
    puts -nonewline $CXX "\t\t\{"
    puts -nonewline $CXX [EventID2LongLongHEX [[$m getElementsByTagName normal -depth 1] data]]
    puts -nonewline $CXX {, }
    puts -nonewline $CXX [EventID2LongLongHEX [[$m getElementsByTagName reverse -depth 1] data]]
    puts $CXX "\},"
    set p [$t getElementsByTagName points -depth 1]
    puts -nonewline $CXX "\t\t\{"
    puts -nonewline $CXX [EventID2LongLongHEX [[$p getElementsByTagName normal -depth 1] data]]
    puts -nonewline $CXX {, }
    puts -nonewline $CXX [EventID2LongLongHEX [[$p getElementsByTagName reverse -depth 1] data]]
    puts $CXX "\}"
    puts -nonewline $CXX "\t\},"
    puts $CXX [format {// %s} [[$t getElementsByTagName name -depth 1] data]]    
}
puts $CXX "\};"
puts $CXX "const char *TurnoutNames\[NUM_TURNOUTS\] = \{"
foreach t [$db getAllTurnouts] {
    set nt [$t getElementsByTagName name -depth 1]
    puts $CXX [format "\t\"%s\"," [$nt data]]
}
puts $CXX "\};"


puts $HXX "enum SignalIndexes"
puts $HXX "\{"
set AspectCount 0
set AspectNames [list]
foreach s [$db getAllSignals] {
    set nt [$s getElementsByTagName name -depth 1]
    puts $HXX [format "\t%s," [Name2Symbol [$nt data]]]
    set aspects [$s getElementsByTagName aspect -depth 1]
    set ac [llength $aspects]
    if {$ac > $AspectCount} {set AspectCount $ac}
    foreach a $aspects {
        set an [[$a getElementsByTagName name -depth 1] data]
        if {[lsearch -exact $AspectNames $an] < 0} {
            lappend AspectNames $an
        }
    }
}
puts $HXX "\tNUM_SIGNALS"
puts $HXX "\};"

puts $HXX "struct SignalConfig \{"
puts $HXX "\tstatic constexpr uint ASPECTCOUNT = $AspectCount;"
puts $HXX "\tenum Aspect"
puts $HXX "\t{"
foreach an $AspectNames {
    puts $HXX [format "\t\t%s," [Name2Symbol $an]]
}
puts $HXX "\t\tUNUSED"
puts $HXX "\t\};"
puts $HXX "\tstruct \{"
puts $HXX "\t\tAspect aspect;"
puts $HXX "\t\topenlcb::EventId eventid;"
puts $HXX "\t\} aspects\[ASPECTCOUNT\];"
puts $HXX "\};"

puts $HXX "extern const SignalConfig SignalsCfg\[NUM_SIGNALS\];"
puts $HXX "extern const char *SignalNames\[NUM_SIGNALS\];"

puts $CXX "const SignalConfig SignalsCfg\[NUM_SIGNALS\] = \{"
foreach s [$db getAllSignals] {
    set aspects [$s getElementsByTagName aspect -depth 1]
    set ac 0
    puts $CXX "\t\{\{"
    foreach a $aspects {
        set an [[$a getElementsByTagName name -depth 1] data]
        set ae [[$a getElementsByTagName event -depth 1] data]
        puts $CXX [format "\t\t{SignalConfig::%s, %s}," [Name2Symbol $an] [EventID2LongLongHEX $ae]]
        incr ac
    }
    while {$ac < $AspectCount} {
        puts $CXX "\t\t{SignalConfig::UNUSED, 0LL},"
        incr ac
    }
    puts -nonewline $CXX "\t\}\},"
    puts $CXX [format {// %s} [[$s getElementsByTagName name -depth 1] data]]    
}
puts $CXX "\};"

puts $CXX "const char *SignalNames\[NUM_SIGNALS\] = \{"
foreach s [$db getAllSignals] {
    set nt [$s getElementsByTagName name -depth 1]
    puts $CXX [format "\t\"%s\"," [$nt data]]
}
puts $CXX "\};"



puts $HXX "\}"
puts $HXX "#endif"
close $HXX

puts $CXX "\}"
close $CXX

