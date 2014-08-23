/*=============================================================================

Copyright (c) 2013 Ville Ruusutie

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

=============================================================================*/
#include "unitos/unitos.h"
#include "containos/delegate.h"
#include "containos/list.h"

namespace c = containos;

namespace {

struct TestAllocator : c::Allocator
{
    virtual void* alloc(size_t size, size_t align)   { align; return ::malloc(size); }
    virtual void  dealloc(void* ptr)                 { ::free(ptr); }
};
//-----------------------------------------------------------------------------

class TestPublisher
{
public:
    c::Delegate<void(int)> onEvent;

    void publish(const int value)                 { onEvent(value); }
};
//-----------------------------------------------------------------------------

class TestReceiver
{
public:
    TestReceiver(int v = 1337) : value(v)   {}

    void handleEvent(int newValue)          { value = newValue; }

    int value;
};
//-----------------------------------------------------------------------------

}

TEST_SUITE(Delegate)
{
    TEST(Invalid)
    {
        c::Delegate<void()> dummy;
        EXPECT_EQUAL(dummy.isValid(), false);
    }

    TEST(PublishReceive)
    {
        TestPublisher publisher;
        TestReceiver receiver;
        publisher.onEvent.set(&receiver,&TestReceiver::handleEvent);
        EXPECT_EQUAL(receiver.value, 1337);
        publisher.publish(777);
        EXPECT_EQUAL(receiver.value, 777);
    }

    TEST(StatelessLambda1)
    {
        c::Delegate<int()> onEvent;
        onEvent.set([]() -> int { return 99; });
        int result = onEvent();
        EXPECT_EQUAL(result, 99);
    }

    TEST(StatelessLambda2)
    {
        c::Delegate<int(int,int)> onEvent;
        onEvent.set([](int a, int b) -> int { return a + b; });
        int result = onEvent(11, 7);
        EXPECT_EQUAL(result, 18);
    }

    TEST(StatelessLambda3)
    {
        c::Delegate<int(int,int,int)> onEvent;
        onEvent.set([](int a, int b, int c) -> int { return a + b + c; });
        int result = onEvent(13, 21, 6);
        EXPECT_EQUAL(result, 40);
    }

    TEST(StatelessLambda4)
    {
        c::Delegate<int(int,int,int,int)> onEvent;
        onEvent.set([](int a, int b, int c, int d) -> int { return a + b + c + d; });
        int result = onEvent(13, 21, 6, 5);
        EXPECT_EQUAL(result, 45);
    }

    TEST(StatefulLambda)
    {
        TestAllocator allocator;

        int value = 123;
        c::Delegate<int()> onEvent;
        onEvent.set([&]() -> int { return value; }, &allocator);
        int result = onEvent();
        EXPECT_EQUAL(result, 123);
        value = 77;
        result = onEvent();
        EXPECT_EQUAL(result, 77);
    }

    TEST(StatefulLambdaModify)
    {
        TestAllocator allocator;

        int value = 123;
        c::Delegate<void(int)> onEvent;
        onEvent.set([&](int x) { value = x; }, &allocator);
        EXPECT_EQUAL(value, 123);
        onEvent(77);
        EXPECT_EQUAL(value, 77);
    }

    TEST(ListReferenceLambda)
    {
        TestAllocator allocator;

        c::List<int> list;
        c::Delegate<void(int)> onEvent;
        onEvent.set([&](int i) { list.insert(i); }, &allocator);
        onEvent(3);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 3);
        onEvent(99);
        EXPECT_EQUAL(list.size(), 2u);
        EXPECT_EQUAL(list[1], 99);
    }

    TEST(CopyStatelessLambda)
    {
        c::Delegate<int()> onEvent;
        onEvent.set([]() -> int { return 99; });
        c::Delegate<int()> onEventCopy(onEvent);
        int result = onEventCopy();
        EXPECT_EQUAL(result, 99);
    }

    /*TEST(CopyStatefulLambda)
    {
        TestAllocator allocator;

        int value = 123;
        c::Delegate<void(int)> onEvent;
        onEvent.set([&](int x) { value = x; }, &allocator);
        c::Delegate<void(int)> onEventCopy(onEvent);
        EXPECT_EQUAL(value, 123);
        onEventCopy(77);
        EXPECT_EQUAL(value, 77);
    }*/
}
