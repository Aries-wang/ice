// **********************************************************************
//
// Copyright (c) 2003
// ZeroC, Inc.
// Billerica, MA, USA
//
// All Rights Reserved.
//
// Ice is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// **********************************************************************

#ifndef TEST_ICE
#define TEST_ICE

class C {};

sequence<C> CSeq;
["java:java.util.ArrayList"] sequence<C> CArray;
["java:java.util.LinkedList"] sequence<C> CList;

["java:java.util.ArrayList"] sequence<bool> BoolSeq;
["java:java.util.ArrayList"] sequence<byte> ByteSeq;
["java:java.util.ArrayList"] sequence<short> ShortSeq;
["java:java.util.ArrayList"] sequence<int> IntSeq;
["java:java.util.ArrayList"] sequence<long> LongSeq;
["java:java.util.ArrayList"] sequence<float> FloatSeq;
["java:java.util.ArrayList"] sequence<double> DoubleSeq;
["java:java.util.ArrayList"] sequence<string> StringSeq;

enum E { E1, E2, E3 };
["java:java.util.ArrayList"] sequence<E> ESeq;

struct S
{
    E en;
};
["java:java.util.ArrayList"] sequence<S> SSeq;

dictionary<int, string> D;
["java:java.util.ArrayList"] sequence<D> DSeq;

sequence<StringSeq> StringSeqSeq;

class Test
{
    CSeq opCSeq(CSeq inSeq, out CSeq outSeq);
    CArray opCArray(CArray inSeq, out CArray outSeq);
    CList opCList(CList inSeq, out CList outSeq);
    BoolSeq opBoolSeq(BoolSeq inSeq, out BoolSeq outSeq);
    ByteSeq opByteSeq(ByteSeq inSeq, out ByteSeq outSeq);
    ShortSeq opShortSeq(ShortSeq inSeq, out ShortSeq outSeq);
    IntSeq opIntSeq(IntSeq inSeq, out IntSeq outSeq);
    LongSeq opLongSeq(LongSeq inSeq, out LongSeq outSeq);
    FloatSeq opFloatSeq(FloatSeq inSeq, out FloatSeq outSeq);
    DoubleSeq opDoubleSeq(DoubleSeq inSeq, out DoubleSeq outSeq);
    StringSeq opStringSeq(StringSeq inSeq, out StringSeq outSeq);
    ESeq opESeq(ESeq inSeq, out ESeq outSeq);
    SSeq opSSeq(SSeq inSeq, out SSeq outSeq);
    DSeq opDSeq(DSeq inSeq, out DSeq outSeq);
    StringSeqSeq opStringSeqSeq(StringSeqSeq inSeq, out StringSeqSeq outSeq);

    void shutdown();

    ["java:java.util.ArrayList"] CSeq seq;
};

#endif
