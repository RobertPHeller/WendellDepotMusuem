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
//  Last Modified : <250225.1536>
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
#include "utils/logging.h"
#include <string>
#include <map>
#include <regex>
#include <unistd.h>
#include <stdio.h>

using String = std::string;

WendellDepotWebserver::WendellDepotWebserver(ExecutorBase *executor, 
                                             uint16_t port,
                                             const char *doc_root,
                                             RunATrainFlow &trainFlow)
      : server_(executor,port)
, docRoot_(doc_root)
, trainRunner_(nullptr)
, status_(idle)
, trainFlow_(trainFlow)
{
    server_.add_uri(HTTPD::Uri("/"),[](const HTTPD::HttpRequest *request, 
                                       HTTPD::HttpReply *reply,
                                       void *userContext)
                {
                    ((WendellDepotWebserver *)userContext)->homepageHandler(request,reply);
                },this);
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

const WendellDepotWebserver::FunctionMap_t WendellDepotWebserver::FunctionMap = {
    {
        "QueueTrains", 
        WendellDepotWebserver::QueueTrains
    },
    {
        "RunningTrains",
        WendellDepotWebserver::RunningTrains
    }
};

void WendellDepotWebserver::commandUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply)
{
    string Uri = request->RequestUri();
    string Query = request->Query();
    ParseQuery formdata(Query);
    switch (FindFunction(formdata.Value("function")))
    {
    case QueueTrains:
        {
            RunTrain *trains = new RunTrain[RunTrain::NUM_ROUTES];
            trains[0].address = formdata.IValue("train1");
            trains[0].route = (RunTrain::Route) formdata.IValue("route1");
            trains[1].address = formdata.IValue("train2");
            trains[1].route = (RunTrain::Route) formdata.IValue("route2");
            trains[2].address = formdata.IValue("train3");
            trains[2].route = (RunTrain::Route) formdata.IValue("route3");
            trains[3].address = formdata.IValue("train4");
            trains[3].route = (RunTrain::Route) formdata.IValue("route4");
            uint numtrains = 4;
            uint loopcount = formdata.IValue("loopcount");
            trainRunner_ = new RunTrains(&server_,trains,numtrains,
                                         loopcount,trainFlow_,this);
            delete[] trains;
            status_ = runningtrains;
            reply->SetStatus(302);
            reply->SetHeader("Location","/command?function=RunningTrains");
        }
        break;
    case RunningTrains:
        if (status_ != runningtrains)
        {
            reply->SetStatus(302);
            reply->SetHeader("Location","/");
        }
        else
        {
            reply->SetStatus(200);
            reply->SetContentType("text/html");
            reply->Puts("<HTML><HEAD><TITLE>Trains Are Running</TITLE>\r\n");
            reply->Puts("<link rel='stylesheet' href='/CSS/WendellDepot.css' />\r\n");
            reply->Puts("<script id='WendellDepot' type='text/javascript' src='/JS/WendellDepot.js'></script>\r\n");
            reply->Puts("</HEAD>\r\n<BODY>\r\n");
            reply->Puts("<FORM  ACTION='/command' METHOD='GET'>\r\n");
            reply->Puts(String("<SPAN>Currently running train ")+std::to_string(trainRunner_->CurrentTrainAddress())+" on route");
            switch (trainRunner_->CurrentTrainRoute())
            {
            case RunTrain::EastBound3: reply->Puts("East Bound from track 3"); break;
            case RunTrain::EastBound1: reply->Puts("East Bound from track 1"); break;
            case RunTrain::WestBound4: reply->Puts("West Bound from track 4"); break;
            case RunTrain::WestBound2: reply->Puts("West Bound from track 2"); break;
            default: break;
            }
            reply->Puts("</SPAN>\r\n");
            reply->Puts("<BUTTON TYPE='SUBMIT' NAME='function' VALUE='RunningTrains'>Refresh</BUTTON>\r\n");
            reply->Puts("<BUTTON TYPE='SUBMIT' NAME='function' VALUE='Cancel'>Cancel</BUTTON>\r\n");
            reply->Puts("</FORM></BODY></HTML>");
        }
        break;
    case NoFunction:
        reply->SetStatus(500);
        reply->SetContentType("text/html");
        reply->Puts("<HTML><HEAD><TITLE>Unknown Function</TITLE></HEAD>\r\n");
        reply->Puts(String("<BODY><H1>")+Uri+"</H1>\r\n");
        for (auto i = formdata.begin(); i != formdata.end(); i++)
        {
            reply->Puts(String("<p>")+i->first+" = "+i->second+"</p>");
        }
        reply->Puts("</body></html>");
        break;
    }
    reply->SendReply();
}

void WendellDepotWebserver::staticFileUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply)
{
    String path = docRoot_ + request->RequestUri();
    LOG(ALWAYS,"*** WendellDepotWebserver::staticFileUriHandler() path is '%s'\n",path.c_str());
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


                    
WendellDepotWebserver::ParseQuery::ParseQuery(const String queryString)
{
    size_t amp = 0;
    for (size_t istart=0;
         istart < queryString.length(); 
         istart = amp+1)
    {
        //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery: istart = %d",(int)istart);
        amp = queryString.find('&',istart);
        //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery: amp = %d",(int)amp);
        if (amp == String::npos)
        {
            amp = queryString.length();
        }
        String q = queryString.substr(istart,amp-istart);
        //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery: q = '%s'",q.c_str());
        size_t eq = q.find('=');
        String name = q.substr(0,eq);
        String value = q.substr(eq+1);
        //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery: name = '%s' and value = '%s'",name.c_str(),value.c_str());
        parsedFormData_.insert(std::pair<String,String>(name,unquoteInput_(value)));
    }
}

const String WendellDepotWebserver::ParseQuery::unquoteInput_(const String s) const
{
    //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery::unquoteInput_(%s)",s.c_str());
    std::regex newline("%0d%0a", std::regex_constants::icase);
    String result = std::regex_replace(s,newline,"\n");
    std::regex percentEscapes("%[a-f0-9][a-f0-9]",std::regex_constants::icase);
    String temp = result;
    auto percent_begin = std::sregex_iterator(temp.begin(),temp.end(),percentEscapes);
    auto percent_end = std::sregex_iterator();
    //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery::unquoteInput_: %d percent escapes",(int) std::distance(percent_begin,percent_end));
    if (std::distance(percent_begin,percent_end) > 0)
    {
        result = "";
        String lastSuffix = "";
        //int index = 0;
        for (std::sregex_iterator i = percent_begin; i != percent_end; ++i)
        {
            std::smatch match = *i;
            //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery::unquoteInput_: match %d: str is '%s', prefix is '%s', suffix is '%s'",
            //    index++,match.str().c_str(),match.prefix().str().c_str(),match.suffix().str().c_str());
            result += match.prefix().str();
            size_t unused = 0;
            result += (char)std::stoi(match.str().substr(1),&unused,16);
            lastSuffix = match.suffix().str();
        }
        result += lastSuffix;
        //LOG(ALWAYS,"*** WendellDepotWebserver::ParseQuery::unquoteInput_: result is '%s'",result.c_str());
    }
    return result;
}

void WendellDepotWebserver::homepageHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply)
{
    if (status_ == idle)
    {
        reply->SetStatus(302);
        reply->SetHeader("Location","/index.html");
        reply->SendReply();
    }
    else if (status_ == runningtrains)
    {
        reply->SetStatus(302);
        reply->SetHeader("Location","/command?function=RunningTrains");
        reply->SendReply();
    }
}

WendellDepotWebserver::RunTrains::RunTrains(Service *service,
                                            RunTrain *trains, uint count, 
                                            uint loopcount, 
                                            RunATrainFlow &trainFlow, 
                                            WendellDepotWebserver* parent)
      : StateFlowBase(service)
, numTrains_(count)
, loopcount_(loopcount)
, trainFlow_(trainFlow)
, parent_(parent)
{
    for (uint i = 0;i < numTrains_;i++)
    {
        trains_[i].address = trains[i].address;
        trains_[i].route   = trains[i].route;
    }
    start_flow(STATE(entry));
}

WendellDepotWebserver::RunTrains::~RunTrains()
{
    parent_->resetTrainRunner();
}

StateFlowBase::Action WendellDepotWebserver::RunTrains::entry()
{
    LOG(ALWAYS,"WendellDepotWebserver::RunTrains::entry()");
    loopIndex_ = 0;
    trainNum_ = 0;
    return call_immediately(STATE(allocateTrain));
}

StateFlowBase::Action WendellDepotWebserver::RunTrains::allocateTrain()
{
    LOG(ALWAYS,"WendellDepotWebserver::RunTrains::allocateTrain()");
    return allocate_and_call(&trainFlow_,STATE(startTrain));
}

StateFlowBase::Action WendellDepotWebserver::RunTrains::startTrain()
{
    LOG(ALWAYS,"WendellDepotWebserver::RunTrains::startTrain()");
    Buffer<RunTrain> *buffer = get_allocation_result(&trainFlow_);
    buffer->data()->address = trains_[trainNum_].address;
    buffer->data()->route   = trains_[trainNum_].route;
    buffer->data()->done.reset(this);
    if (buffer->data()->address == 0)
    {
        trainNum_++;
        if (trainNum_ < numTrains_)
        {
            return again();
        }
        else
        {
            trainNum_ = 0;
            loopIndex_++;
            
            if (loopcount_ > 0 && loopIndex_ >= loopcount_)
            {
                return call_immediately(STATE(delete_this));
            }
            else
            {
                return again();
            }
        }
    }
    else
    {
        LOG(ALWAYS,"WendellDepotWebserver::RunTrains::startTrain(): buffer->data()->address is %d, buffer->data()->route is %d",buffer->data()->address,buffer->data()->route);
        trainFlow_.send(buffer);
        return wait_and_call(STATE(nextTrain));
    }
}

StateFlowBase::Action WendellDepotWebserver::RunTrains::nextTrain()
{
    LOG(ALWAYS,"WendellDepotWebserver::RunTrains::nextTrain()");
    trainNum_++;
    if (trainNum_ < numTrains_)
    {        
        return call_immediately(STATE(allocateTrain));
    }
    else
    {
        trainNum_ = 0;
        loopIndex_++;
        if (loopcount_ > 0 && loopIndex_ >= loopcount_)
        {
            return call_immediately(STATE(delete_this));
        }
        else
        {
            return call_immediately(STATE(allocateTrain));
        }
    }
    // return call_immediately(STATE(delete_this)); // never gets here. Keep the compiler happy.
}
