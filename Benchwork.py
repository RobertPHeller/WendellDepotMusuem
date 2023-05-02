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
#  Last Modified : <230502.1420>
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
    
class Plywood(object):
    def __init__(self,name,origin,thickness=3/8,width=48,length=96,cutlist=[]):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.thickness = thickness
        self.width = width
        self.length = length
        self._body =  Part.makePlane(self.length,self.width,self.origin)\
                        .extrude(Base.Vector(0,0,self.thickness))
        for c in cutlist:
            self._body = self._body.cut(c)
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape=self._body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([210/255,180/255,140/255])
        

class PinkFoam(object):
    def __init__(self,name,origin,thickness=2,width=48,length=96,cutlist=[]):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.thickness = thickness
        self.width = width
        self.length = length
        self._body =  Part.makePlane(self.length,self.width,self.origin)\
                        .extrude(Base.Vector(0,0,self.thickness))
        for c in cutlist:
            self._body = self._body.cut(c)
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape=self._body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([255/255,182/255,193/255])

class MasoniteBackdrop(object):
    _height=24
    _thickness=1/8
    def __init__(self,name,origin,length,orientation='horizontal'):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        self.length = length
        self.orientation = orientation
        if self.orientation == 'horizontal':
            self._body =  Part.makePlane(self.length,self._thickness,self.origin)\
                        .extrude(Base.Vector(0,0,self._height))
        elif self.orientation == 'vertical':
            self._body =  Part.makePlane(self._thickness,self.length,self.origin)\
                        .extrude(Base.Vector(0,0,self._height))
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape=self._body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([165/255,42/255,42/255])

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
        self._leg1 = L4x4Leg("leg1",self.origin,self._offFloor,\
                             [self._l1._body,self._h1._body])
        
        leg2O = self.origin.add(Base.Vector(0,\
                                            self._totalHeight-L4x4Leg.Width(),\
                                            0))
        self._leg2 = L4x4Leg("leg2",leg2O,self._offFloor,\
                             [self._l2._body,self._h1._body])
        leg3O = self.origin.add(Base.Vector((self._totalWidth/2)-(L4x4Leg.Width()/2),
                                            0,0))
        self._leg3 = L4x4Leg("leg3",leg3O,self._offFloor,\
                             [self._l1._body,self._h4._body])
        leg4O = self.origin.add(Base.Vector((self._totalWidth/2)-(L4x4Leg.Width()/2),
                                            self._totalHeight-L4x4Leg.Width(),\
                                            0))
        self._leg4 = L4x4Leg("leg4",leg4O,self._offFloor,\
                             [self._l2._body,self._h4._body])

        leg5O = self.origin.add(Base.Vector(self._totalWidth-L4x4Leg.Width(),
                                            0,0))
        self._leg5 = L4x4Leg("leg5",leg5O,self._offFloor,\
                             [self._l1._body,self._h8._body])
        
        leg6O = self.origin.add(Base.Vector(self._totalWidth-L4x4Leg.Width(),\
                                            self._totalHeight-L4x4Leg.Width(),\
                                            0))
        self._leg6 = L4x4Leg("leg6",leg6O,self._offFloor,\
                             [self._l2._body,self._h8._body])
        sheet1O = self.origin.add(Base.Vector(0,0,self._offFloor))
        self._sheet1 = Plywood("sheet1",sheet1O)
        sheet2O = sheet1O.add(Base.Vector(self._sheet1.length,0,0))
        self._sheet2 = Plywood("sheet2",sheet2O,3/8,48,\
                                self._totalWidth-self._sheet1.length)
        sheet3O = self.origin.add(Base.Vector(0,self._sheet1.width,\
                                                self._offFloor))
        self._sheet3 = Plywood("sheet3",sheet3O,3/8,\
                                self._totalHeight-self._sheet1.width)
        sheet4O = sheet1O.add(Base.Vector(self._sheet1.length,self._sheet2.width,0))
        self._sheet4 = Plywood("sheet4",sheet4O,3/8,\
                                self._totalHeight-self._sheet2.width,\
                                self._totalWidth-self._sheet1.length)

        foam1O = self.origin.add(Base.Vector(0,0,\
                                self._offFloor+self._sheet1.thickness))
        self._foam1 = PinkFoam("foam1",foam1O)
        foam2O = foam1O.add(Base.Vector(self._foam1.length,0,0))
        self._foam2 = PinkFoam("foam2",foam2O,2,48,\
                                self._totalWidth-self._foam1.length)
        foam3O = self.origin.add(Base.Vector(0,self._foam1.width,
                                self._offFloor+self._sheet3.thickness))
        self._foam3 = PinkFoam("foam3",foam3O,2,\
                                self._totalHeight-self._foam1.width)
        foam4O = foam1O.add(Base.Vector(self._foam1.length,self._foam2.width,0))
        self._foam4 = PinkFoam("foam4",foam4O,2,\
                                self._totalHeight-self._foam2.width,\
                                self._totalWidth-self._foam1.length)
        m1O = Base.Vector(h2O.x+L2x4.Width(),30,foam1O.z+self._foam1.thickness)
        self._m1 = MasoniteBackdrop("m1",m1O,self._totalHeight-30,'vertical')
        self._m2 = MasoniteBackdrop("m2",m1O,10*12,'horizontal')
        m3O = m1O.add(Base.Vector(10*12,0,0))
        self._m3 = MasoniteBackdrop("m3",m3O,self._totalHeight-30,'vertical')
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
        self._sheet1.show()
        self._sheet2.show()
        self._sheet3.show()
        self._sheet4.show()
        self._foam1.show()
        self._foam2.show()
        self._foam3.show()
        self._foam4.show()
        self._m1.show()
        self._m2.show()
        self._m3.show()

if __name__ == '__main__':
    App.ActiveDocument=App.newDocument("Temp")
    doc = App.activeDocument()
    benchwork = WendellDepot_Benchwork("benchwork",Base.Vector(0,0,0))
    benchwork.show()
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewTop()
    
        
