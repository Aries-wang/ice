// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <TestI.h>
#include <Ice/Ice.h>

using namespace std;
using namespace Ice;

TestIntfI::TestIntfI() :
    _batchCount(0)
{
}

void
TestIntfI::op(const Ice::Current&)
{
}

int
TestIntfI::opWithResult(const Ice::Current&)
{
    return 15;
}

void
TestIntfI::opWithUE(const Ice::Current&)
{
    throw Test::TestIntfException();
}

int
TestIntfI::opWithResultAndUE(const Ice::Current&)
{
    throw Test::TestIntfException();
}

void
TestIntfI::opWithPayload(ICE_IN(Ice::ByteSeq), const Ice::Current&)
{
}

void
TestIntfI::opBatch(const Ice::Current&)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
    ++_batchCount;
    notify();
}

Ice::Int
TestIntfI::opBatchCount(const Ice::Current&)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
    return _batchCount;
}

void
TestIntfI::opWithArgs(Ice::Int& one, Ice::Int& two, Ice::Int& three, Ice::Int& four, Ice::Int& five, Ice::Int& six,
                      Ice::Int& seven, Ice::Int& eight, Ice::Int& nine, Ice::Int& ten, Ice::Int& eleven,
                      const Ice::Current&)
{
    one = 1;
    two = 2;
    three = 3;
    four = 4;
    five = 5;
    six = 6;
    seven = 7;
    eight = 8;
    nine = 9;
    ten = 10;
    eleven = 11;
}

bool
TestIntfI::waitForBatch(Ice::Int count, const Ice::Current&)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
    while(_batchCount < count)
    {
        timedWait(IceUtil::Time::milliSeconds(5000));
    }
    bool result = count == _batchCount;
    _batchCount = 0;
    return result;
}

void
TestIntfI::close(Test::CloseMode mode, const Ice::Current& current)
{
    current.con->close(static_cast<ConnectionClose>(mode));
}

void
TestIntfI::sleep(Ice::Int ms, const Ice::Current& current)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
    timedWait(IceUtil::Time::milliSeconds(ms));
}

#ifdef ICE_CPP11_MAPPING
void
TestIntfI::startDispatchAsync(std::function<void()> response, std::function<void(std::exception_ptr)> ex,
                              const Ice::Current&)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
    _pending.push_back(move(response));
}
#else
void
TestIntfI::startDispatch_async(const Test::AMD_TestIntf_startDispatchPtr& cb, const Ice::Current&)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
    _pending.push_back(cb);
}
#endif

void
TestIntfI::finishDispatch(const Ice::Current& current)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
#ifdef ICE_CPP11_MAPPING
    for(vector<function<void()>>::iterator p = _pending.begin(); p != _pending.end(); ++p)
    {
        (*p)();
    }
#else
    for(vector<Test::AMD_TestIntf_startDispatchPtr>::iterator p = _pending.begin(); p != _pending.end(); ++p)
    {
        (*p)->ice_response();
    }
#endif
    _pending.clear();
}

void
TestIntfI::shutdown(const Ice::Current& current)
{
    //
    // Just in case a request arrived late.
    //
    IceUtil::Monitor<IceUtil::Mutex>::Lock sync(*this);
#ifdef ICE_CPP11_MAPPING
    for(vector<function<void()>>::iterator p = _pending.begin(); p != _pending.end(); ++p)
    {
        (*p)();
    }
#else
    for(vector<Test::AMD_TestIntf_startDispatchPtr>::iterator p = _pending.begin(); p != _pending.end(); ++p)
    {
        (*p)->ice_response();
    }
#endif
    current.adapter->getCommunicator()->shutdown();
}

bool
TestIntfI::supportsAMD(const Ice::Current&)
{
    return true;
}

bool
TestIntfI::supportsFunctionalTests(const Ice::Current&)
{
    return false;
}

void
TestIntfControllerI::holdAdapter(const Ice::Current&)
{
    _adapter->hold();
}

void
TestIntfControllerI::resumeAdapter(const Ice::Current&)
{
    _adapter->activate();
}

TestIntfControllerI::TestIntfControllerI(const Ice::ObjectAdapterPtr& adapter) : _adapter(adapter)
{
}
