/*=============================================================================

Copyright (c) 2007-2013 Ville Ruusutie

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
#include "containos/singleton.h"

namespace c = containos;

class TestSingleton : public c::Singleton<TestSingleton>
{
public:
    TestSingleton() : value(1337) {}
    ~TestSingleton() { }
    int value;
};

TEST_SUITE(Singleton)
{
    TEST(Invalid)
    {
        EXPECT_EQUAL(TestSingleton::hasInstance(), false);
    }
    
    TEST(CreateDelete)
    {
        EXPECT_EQUAL(TestSingleton::hasInstance(), false);
        TestSingleton::createInstance();
        EXPECT_EQUAL(TestSingleton::hasInstance(), true);
        EXPECT_EQUAL(TestSingleton::getInstance().value, 1337);
        TestSingleton::deleteInstance();
        EXPECT_EQUAL(TestSingleton::hasInstance(), false);
    }
}
