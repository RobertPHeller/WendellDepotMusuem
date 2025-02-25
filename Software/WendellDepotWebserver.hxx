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
//  Last Modified : <250225.1534>
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
#include "executor/StateFlow.hxx"
#include "httpd/Httpd.hxx"
#include "httpd/HttpRequest.hxx"
#include "httpd/HttpReply.hxx"
#include "httpd/Uri.hxx"
#include "RunATrain.hxx"
#include <string>
#include <map>

using String = std::string;

class WendellDepotWebserver {
public:
    WendellDepotWebserver(ExecutorBase *executor,
                          uint16_t port, const char *doc_root, 
                          RunATrainFlow &trainFlow);
    ~WendellDepotWebserver() {}
    typedef std::map<String,String> FormData_t;
    enum Function_t {NoFunction = -1, QueueTrains, RunningTrains};
private:
    typedef std::map<String,Function_t> FunctionMap_t;
    static const FunctionMap_t FunctionMap;
    static inline Function_t FindFunction(const String f)
    {
        auto i = FunctionMap.find(f);
        if (i == FunctionMap.end())
        {
            return NoFunction;
        }
        else
        {
            return i->second;
        }
    }
    class ParseQuery {
    public:
        ParseQuery(const String queryString);
        const String Value(const String name) const {
            auto i = parsedFormData_.find(name);
            if (i == parsedFormData_.end()) {
                return "";
            }
            else
            {
                return i->second;
            }
        }
        int IValue(const String name) const {
            const String s = Value(name);
            if (s == "")
            {
                return 0;
            }
            else
            {
                try 
                {
                    int result = std::stoi(s);
                    return result;
                }
                catch (const std::exception& e) 
                {
                    return 0;
                }
            }
        }
        const FormData_t::const_iterator begin() const {return parsedFormData_.cbegin();}
        const FormData_t::const_iterator end() const {return parsedFormData_.cend();}
    private:
        FormData_t parsedFormData_;
        const String unquoteInput_(const String s) const;
    };
    class RunTrains : public StateFlowBase {
    public:
        RunTrains(Service *service, RunTrain *trains, uint count, 
                  uint loopcount, RunATrainFlow &trainFlow,
                  WendellDepotWebserver* parent);
        virtual ~RunTrains();
        void CancelTrains();
        bool TrainsAreRunning();
        uint16_t CurrentTrainAddress () const 
        {
            if (trainNum_ >= RunTrain::NUM_ROUTES) 
            {
                return 0;
            }
            else
            {
                return trains_[trainNum_].address;
            }
        }
        RunTrain::Route CurrentTrainRoute () const
        {
            if (trainNum_ >= RunTrain::NUM_ROUTES)
            {
                return RunTrain::NUM_ROUTES;
            }
            else
            {
                return trains_[trainNum_].route;
            }
        }
    private:
        RunTrain trains_[RunTrain::NUM_ROUTES];
        uint numTrains_;
        uint loopcount_;
        RunATrainFlow &trainFlow_;
        WendellDepotWebserver *parent_;
        uint loopIndex_;
        uint trainNum_;
        virtual Action entry();
        Action allocateTrain();
        Action startTrain();
        Action nextTrain();
        
    };
    friend class RunTrains;
    HTTPD::Httpd server_;
    String docRoot_;
    RunTrains *trainRunner_;
    enum Status_t {idle, runningtrains} status_;
    RunATrainFlow &trainFlow_;
    void resetTrainRunner()
    {
        trainRunner_ = nullptr;
        status_ = idle;
    }
    void staticFileUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply);
    void commandUriHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply);
    void homepageHandler(const HTTPD::HttpRequest *request, HTTPD::HttpReply *reply);
};
    

#endif // __WENDELLDEPOTWEBSERVER_HXX

