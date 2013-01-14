// **********************************************************************
//
// Copyright (c) 2003-2012 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

package com.zeroc.ejb;

import javax.ejb.*;

import com.zeroc.ice.Test.Account;
import com.zeroc.ice.Test._ServiceDisp;

//
// This Ice servant delegates the calls to the Service EJB
//
public class ServiceI extends _ServiceDisp
{
    final private Service service;

    public ServiceI(Service service)
    {
        this.service = service;
    }

    public final Account 
    getAccount(Ice.Current current)
    {
        return service.getAccount();
    }

    public final void
    setAccount(Account s, Ice.Current current)
    {
        service.setAccount(s);
    }
}