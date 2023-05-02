#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Tue May 2 11:10:49 2023
#  Last Modified : <230502.1230>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
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


import Part
from FreeCAD import Base
import FreeCAD as App
import os
import sys
sys.path.append(os.path.dirname(__file__))
import Mesh

import datetime

from abc import ABCMeta, abstractmethod, abstractproperty

class L2x4(object):
    @staticmethod
    def Width():
        return 1.5
    def Height():
        return 3.5
    def __init__(self,name,origin,length,orientation='horizontal'):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.length = length
        self.orientation = orientation
        if orientation == 'horizontal':
            self._body =  Part.makePlane(self.length,L2x4.Width(),self.origin)\
                            .extrude(Base.Vector(0,0,L2x4.Height()))
        elif orientation == 'vertical':
            self._body =  Part.makePlane(L2x4.Width(),self.length,self.origin)\
                            .extrude(Base.Vector(0,0,L2x4.Height()))
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape=self._body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([210/255,180/255,140/255])
        

class L4x4Leg(object):
    @staticmethod
    def Width():
        return 3.5
    def Height():
        return 3.5
    def __init__(self,name,origin,length,cutlist=[]):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.length = length
        self._body =  Part.makePlane(L4x4Leg.Width(),L4x4Leg.Height(),self.origin)\
                .extrude(Base.Vector(0,0,self.length))
        for c in cutlist:
            self._body = self._body.cut(c)
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape=self._body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([210/255,180/255,140/255])
    

class WendellDepot_Benchwork(object):
    _totalWidth=14*12
    _totalHeight=6*12
    _offFloor=40
    _spacing=24
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        l1O=self.origin.add(Base.Vector(0,\
                                        0,\
                                        self._offFloor-L2x4.Height()))
        self._l1 = L2x4("l1",l1O,self._totalWidth,'horizontal')
        l2O=self.origin.add(Base.Vector(0,\
                                        self._totalHeight-L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._l2 = L2x4("l2",l2O,self._totalWidth,'horizontal')
        x=0
        h1O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h1 = L2x4("h1",h1O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing-L2x4.Width()/2.0
        h2O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h2 = L2x4("h2",h2O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing
        h3O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h3 = L2x4("h3",h3O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing
        h4O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h4 = L2x4("h4",h4O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing
        h5O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h5 = L2x4("h5",h5O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing
        h6O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h6 = L2x4("h6",h6O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing
        h7O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h7 = L2x4("h7",h7O,self._totalHeight-L2x4.Width()*2,'vertical')
        x+=self._spacing-L2x4.Width()/2.0
        h8O=self.origin.add(Base.Vector(x,\
                                        L2x4.Width(),\
                                        self._offFloor-L2x4.Height()))
        self._h8 = L2x4("h8",h8O,self._totalHeight-L2x4.Width()*2,'vertical')
        self._leg1 = L4x4Leg("l1",self.origin,self._offFloor,\
                             [self._l1._body,self._h1._body])
        
        leg2O = self.origin.add(Base.Vector(0,\
                                            self._totalHeight-L4x4Leg.Width(),\
                                            0))
        self._leg2 = L4x4Leg("l2",leg2O,self._offFloor,\
                             [self._l2._body,self._h1._body])
        leg3O = self.origin.add(Base.Vector((self._totalWidth/2)-(L4x4Leg.Width()/2),
                                            0,0))
        self._leg3 = L4x4Leg("l3",leg3O,self._offFloor,\
                             [self._l1._body,self._h4._body])
        leg4O = self.origin.add(Base.Vector((self._totalWidth/2)-(L4x4Leg.Width()/2),
                                            self._totalHeight-L4x4Leg.Width(),\
                                            0))
        self._leg4 = L4x4Leg("l4",leg4O,self._offFloor,\
                             [self._l2._body,self._h4._body])

        leg5O = self.origin.add(Base.Vector(self._totalWidth-L4x4Leg.Width(),
                                            0,0))
        self._leg5 = L4x4Leg("l5",leg5O,self._offFloor,\
                             [self._l1._body,self._h8._body])
        
        leg6O = self.origin.add(Base.Vector(self._totalWidth-L4x4Leg.Width(),\
                                            self._totalHeight-L4x4Leg.Width(),\
                                            0))
        self._leg6 = L4x4Leg("l6",leg6O,self._offFloor,\
                             [self._l2._body,self._h8._body])
        
    def show(self):
        self._l1.show()
        self._l2.show()
        self._h1.show()
        self._h2.show()
        self._h3.show()
        self._h4.show()
        self._h5.show()
        self._h6.show()
        self._h7.show()
        self._h8.show()
        self._leg1.show()
        self._leg2.show()
        self._leg3.show()
        self._leg4.show()
        self._leg5.show()
        self._leg6.show()

if __name__ == '__main__':
    App.ActiveDocument=App.newDocument("Temp")
    doc = App.activeDocument()
    benchwork = WendellDepot_Benchwork("benchwork",Base.Vector(0,0,0))
    benchwork.show()
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewTop()
    
        
