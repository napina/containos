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
#include "containos/fixedlist.h"

namespace c = containos;

TEST_SUITE(FixedList)
{
    TEST(Empty)
    {
        c::FixedList<int,4> list;
        EXPECT_EQUAL(list.size(), 0u);
        EXPECT_EQUAL(list.capasity(), 4u);
    }

    TEST(Insert)
    {
        c::FixedList<int,4> list;
        list.insert(3);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 3);
        list.insert(7);
        EXPECT_EQUAL(list.size(), 2u);
        EXPECT_EQUAL(list[1], 7);
    }

    TEST(Acquire)
    {
        c::FixedList<int,4> list;
        list.acquire() = 3;
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 3);
        list.acquire() = 7;
        EXPECT_EQUAL(list.size(), 2u);
        EXPECT_EQUAL(list[1], 7);
    }

    TEST(Remove)
    {
        c::FixedList<int,4> list;
        list.insert(3);
        EXPECT_EQUAL(list.size(), 1u);
        list.remove(0);
        EXPECT_EQUAL(list.size(), 0u);
    }

    TEST(Clear)
    {
        c::FixedList<int,4> list;
        list.insert(3);
        list.insert(7);
        list.insert(4);
        EXPECT_EQUAL(list.size(), 3u);
        list.clear();
        EXPECT_EQUAL(list.size(), 0u);
    }
}
