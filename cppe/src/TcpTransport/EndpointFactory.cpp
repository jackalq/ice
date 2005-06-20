// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/EndpointFactory.h>
#include <Ice/Endpoint.h>
#include <Ice/LocalException.h>
#include <Ice/BasicStream.h>

using namespace std;
using namespace Ice;
using namespace IceInternal;

void IceInternal::incRef(EndpointFactory* p) { p->__incRef(); }
void IceInternal::decRef(EndpointFactory* p) { p->__decRef(); }

IceInternal::EndpointFactory::EndpointFactory(const InstancePtr& instance)
    : _instance(instance)
{
}

IceInternal::EndpointFactory::~EndpointFactory()
{
}

EndpointPtr
IceInternal::EndpointFactory::create(const std::string& str) const
{
    const string delim = " \t\n\r";

    string::size_type beg = str.find_first_not_of(delim);
    if(beg == string::npos)
    {
        EndpointParseException ex(__FILE__, __LINE__);
        ex.str = str;
        throw ex;
    }

    string::size_type end = str.find_first_of(delim, beg);
    if(end == string::npos)
    {
        end = str.length();
    }

    string protocol = str.substr(beg, end - beg);

    if(protocol == "default" || protocol == "tcp")
    {
	return new Endpoint(_instance, str.substr(end));
    }

    EndpointParseException ex(__FILE__, __LINE__);
}

EndpointPtr
IceInternal::EndpointFactory::read(BasicStream* s) const
{
    Short type;
    s->read(type);

    if(type == TcpEndpointType)
    {
        return new Endpoint(s);
    }

    //
    // XXX: What should this do? Old code returned UnknownEndpoint. Maybe that needs
    // to be added back?
    //
    return 0;
}

void
IceInternal::EndpointFactory::destroy()
{
    _instance = 0;
}
