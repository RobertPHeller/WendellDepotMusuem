// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Feb 12 09:22:42 2025
//  Last Modified : <250212.1213>
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
/// @file WendellDepotWebserver.cxx
/// @author Robert Heller
/// @date Wed Feb 12 09:22:42 2025
/// 
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";


#include "openmrn_features.h"
#include "utils/macros.h"
#include "executor/Executor.hxx"
#include "executor/Service.hxx"
#include "httpd/Httpd.hxx"
#include "httpd/HttpRequest.hxx"
#include "httpd/HttpReply.hxx"
#include "httpd/Uri.hxx"
#include "httpd/mimetable.hxx"
#include "WendellDepotWebserver.hxx"
#include <string>
#include <unistd.h>
#include <stdio.h>

using String = std::string;

WendellDepotWebserver::WendellDepotWebserver(ExecutorBase *executor, 
                                             uint16_t port,
                                             const char *doc_root)
      : server_(executor,port)
, docRoot_(doc_root)
{
    server_.add_uri(HTTPD::Uri("/"),[](const HTTPD::HttpRequest *request, 
                                       HTTPD::HttpReply *reply,
                                       void *userContext)
                {
                    reply->SetStatus(302);
                    reply->SetHeader("Location","/index.html");
                    reply->SendReply();
                },NULL);
    server_.add_uri(HTTPD::UriGlob("/command*"),[](const HTTPD::HttpRequest *request,
                                                   HTTPD::HttpReply *reply,
                                                   void *userContext)
                {
                    ((WendellDepotWebserver *)userContext)->commandUriHandler(request,reply);
                },this);
    server_.add_uri(HTTPD::UriGlob("/*"),
                    [](const HTTPD::HttpRequest *request,
                       HTTPD::HttpReply *reply,
                       void *userContext)
                {
                    ((WendellDepotWebserver *)userContext)->staticFileUriHandler(request,reply);
                },this);
}

void WendellDepotWebserver::commandUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply)
{
    string Uri = request->RequestUri();
    string Query = request->Query();
    reply->SetStatus(200);
    reply->SetContentType("text/html");
    reply->Puts("<HTML><HEAD><TITLE>Command</TITLE></HEAD>\r\n");
    reply->Puts(String("<BODY><H1>")+Uri+"</H1>\r\n");
    reply->Puts(String("Query string is '")+Query+"'</BODY></HTML>\r\n");
    reply->SendReply();
}

void WendellDepotWebserver::staticFileUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply)
{
    String path = docRoot_ + request->RequestUri();
    fprintf(stderr,"*** WendellDepotWebserver::staticFileUriHandler() path is '%s'\n",path.c_str());
    if (access(path.c_str(),F_OK))
    {
        reply->SetStatus(404);
        reply->SetContentType("text/html");
        reply->Puts("<HTML><HEAD><TITLE>");
        reply->Puts(request->RequestUri());
        reply->Puts(" Not Found</TITLE></HEAD>\r\n");
        reply->Puts("<BODY><H1>");
        reply->Puts(request->RequestUri());
        reply->Puts(" Not Found</H1></BODY></HTML>");
        reply->SendReply();
        return;
    }
    else if (access(path.c_str(),R_OK))
    {
        reply->SetStatus(403);
        reply->SetContentType("text/html");
        reply->Puts("<HTML><HEAD><TITLE>");
        reply->Puts(request->RequestUri());
        reply->Puts(" Permission Denied</TITLE></HEAD>\r\n");
        reply->Puts("<BODY><H1>");
        reply->Puts(request->RequestUri());
        reply->Puts(" Permission Denied</H1></BODY></HTML>");
        reply->SendReply();
        return;
    }
    size_t dotpos = path.rfind('.');
    String contentType = mime::mimeTable[mime::none].mimeType;
    if (dotpos != String::npos)
    {
        for (int i = 0; i < mime::maxType; i++)
        {
            if (path.substr(dotpos) == mime::mimeTable[i].endsWith)
            {
                contentType = mime::mimeTable[i].mimeType;
                break;
            }
        }
    }
    FILE *fp = fopen(path.c_str(),"rb");
    if (fp != NULL)
    {
        unsigned char buffer[4096];
        size_t count;
        while ((count=fread(buffer,sizeof(unsigned char),4096,fp)) > 0)
        {
            reply->Puts(String((char *)buffer,count));
        }
        fclose(fp);
    }
    reply->SetContentType(contentType);
    reply->SendReply();    
}


                    
