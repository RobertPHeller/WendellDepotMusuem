#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sat Feb 15 13:57:56 2025
#  Last Modified : <250215.1438>
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
# @file LayoutControlDB2TclArrays.tcl
# @author Robert Heller
# @date Sat Feb 15 13:57:56 2025
# 
#
#*****************************************************************************


lappend auto_path /usr/local/lib/MRRSystem;# C++ (binary) packages
lappend auto_path /usr/local/share/MRRSystem;# Tcl (source) packages
package require snit
package require LayoutControlDB 2.0
package require LCC

set db [lcc::LayoutControlDB olddb ../WendellDepot.xml]

set TCL [open "WendellDepot_DB.tcl" w]

proc Name2Symbol {name} {
    return [regsub -all {[[:space:]()-]} $name "_"]
}

puts $TCL "variable opticalSensors -array \{"
foreach s [$db getAllSensors] {
    set nt [$s getElementsByTagName name -depth 1]
    puts $TCL [format "\t%s \{" [Name2Symbol [$nt data]]]
    puts $TCL [format "\t\t\{%s\}" [$nt data]]
    puts $TCL [format "\t\t%s" [[$s getElementsByTagName on -depth 1] data]]
    puts $TCL [format "\t\t%s" [[$s getElementsByTagName off -depth 1] data]]
    puts $TCL "\t\}"
}
puts $TCL "\}"

puts $TCL "variable turnouts -array \{"
foreach t [$db getAllTurnouts] {
    set nt [$t getElementsByTagName name -depth 1]
    puts $TCL [format "\t%s \{" [Name2Symbol [$nt data]]]
    puts $TCL [format "\t\t\{%s\}" [$nt data]]
    set m [$t getElementsByTagName motor -depth 1]
    puts $TCL "\t\t\{"
    puts $TCL [format "\t\t\t%s" [[$m getElementsByTagName normal -depth 1] data]]
    puts $TCL [format "\t\t\t%s" [[$m getElementsByTagName reverse -depth 1] data]]
    puts $TCL "\t\t\} \{"
    set p [$t getElementsByTagName points -depth 1]
    puts $TCL [format "\t\t\t%s" [[$p getElementsByTagName normal -depth 1] data]]
    puts $TCL [format "\t\t\t%s" [[$p getElementsByTagName reverse -depth 1] data]]
    puts $TCL "\t\t\}"
    puts $TCL "\t\}"
}
puts $TCL "\}"

puts $TCL "variable signals -array \{"
foreach s [$db getAllSignals] {
    set nt [$s getElementsByTagName name -depth 1]
    puts $TCL [format "\t%s \{" [Name2Symbol [$nt data]]]
    puts $TCL [format "\t\t\{%s\}" [$nt data]]
    set aspects [$s getElementsByTagName aspect -depth 1]
    puts $TCL "\t\t\{"
    foreach a $aspects {
        set an [[$a getElementsByTagName name -depth 1] data]
        set ae [[$a getElementsByTagName event -depth 1] data]
        puts $TCL [format "\t\t\t\%s" [Name2Symbol $an]]
        puts $TCL [format "\t\t\t\{\{%s\}" $an]
        puts $TCL [format "\t\t\t%s\}" $ae]
    }
    puts $TCL "\t\t\}"
    puts $TCL "\t\}"
}
puts $TCL "\}"
