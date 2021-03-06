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
#include "containos/slotlist.h"
#include <memory>

namespace c = containos;

TEST_SUITE(SlotList)
{
    TEST(Empty)
    {
        c::SlotList<int,c::Mallocator> list;
        EXPECT_EQUAL(list.size(), 0);
    }
#if 0
    TEST(Insert)
    {
        c::SlotList<int,Mallocator> list;
        list.reserve(2);
        list.insert(3);
        EXPECT_EQUAL(list.size(), 1);
        EXPECT_EQUAL(list[0], 3);
        list.insert(7);
        EXPECT_EQUAL(list.size(), 2);
        EXPECT_EQUAL(list[1], 7);
    }

    TEST(InsertOther)
    {
        c::SlotList<int,Mallocator> list;
        list.reserve(3);
        list.insert(3);
        list.insert(5);
        c::List<int,Mallocator> list2;
        list2.reserve(4);
        list2.insert(9);
        list2.insert(list);
        list2.insert(13);
        EXPECT_EQUAL(list2.size(), 4);
        EXPECT_EQUAL(list2[0], 9);
        EXPECT_EQUAL(list2[1], 3);
        EXPECT_EQUAL(list2[2], 5);
        EXPECT_EQUAL(list2[3], 13);
    }

    TEST(Acquire)
    {
        c::SlotList<int,Mallocator> list;
        list.acquire() = 3;
        EXPECT_EQUAL(list.size(), 1);
        EXPECT_EQUAL(list[0], 3);
        list.acquire() = 7;
        EXPECT_EQUAL(list.size(), 2);
        EXPECT_EQUAL(list[1], 7);
    }

    TEST(Remove)
    {
        c::SlotList<int,Mallocator> list;
        list.insert(3);
        EXPECT_EQUAL(list.size(), 1);
        list.remove(0);
        EXPECT_EQUAL(list.size(), 0);
    }

    TEST(Clear)
    {
        c::SlotList<int,Mallocator> list;
        list.insert(3);
        list.insert(7);
        list.insert(4);
        EXPECT_EQUAL(list.size(), 3);
        list.clear();
        EXPECT_EQUAL(list.size(), 0);
    }
#endif
}
