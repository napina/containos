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
#include "containos/event.h"
#include <memory>

namespace c = containos;

namespace {

class TestPublisher
{
public:
    c::Event<void(int)> onEvent;

    void publish(int value)                 { onEvent(value); }
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

TEST_SUITE(Event)
{
    TEST(Invalid)
    {
        c::Event<void()> dummy;
        EXPECT_EQUAL(dummy.isValid(), false);
        dummy();
    }

    TEST(PublishReceive)
    {
        TestPublisher publisher;
        TestReceiver receiver;
        publisher.onEvent.add(&receiver, &TestReceiver::handleEvent);
        EXPECT_EQUAL(receiver.value, 1337);
        publisher.publish(777);
        EXPECT_EQUAL(receiver.value, 777);
    }

    TEST(PublishMultiReceive)
    {
        TestPublisher publisher;
        TestReceiver receiver(33);
        TestReceiver receiver2(88);
        publisher.onEvent.add(&receiver, &TestReceiver::handleEvent);
        publisher.onEvent.add(&receiver2, &TestReceiver::handleEvent);
        EXPECT_EQUAL(receiver.value, 33);
        EXPECT_EQUAL(receiver2.value, 88);
        publisher.publish(999);
        EXPECT_EQUAL(receiver.value, 999);
        EXPECT_EQUAL(receiver2.value, 999);
    }
}
