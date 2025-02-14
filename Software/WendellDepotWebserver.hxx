// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Feb 12 09:10:33 2025
//  Last Modified : <250212.1057>
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
/// @file WendellDepotWebserver.hxx
/// @author Robert Heller
/// @date Wed Feb 12 09:10:33 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

#ifndef __WENDELLDEPOTWEBSERVER_HXX
#define __WENDELLDEPOTWEBSERVER_HXX

#include "openmrn_features.h"
#include "utils/macros.h"
#include "executor/Executor.hxx"
#include "executor/Service.hxx"
#include "httpd/Httpd.hxx"
#include "httpd/HttpRequest.hxx"
#include "httpd/HttpReply.hxx"
#include "httpd/Uri.hxx"
#include <string>

using String = std::string;

class WendellDepotWebserver {
public:
    WendellDepotWebserver(ExecutorBase *executor, uint16_t port, 
                          const char *doc_root);
    ~WendellDepotWebserver() {}
private:
    HTTPD::Httpd server_;
    String docRoot_;
    void staticFileUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply);
    void commandUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply);
};
    

#endif // __WENDELLDEPOTWEBSERVER_HXX

