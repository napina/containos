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
#include "containos/list.h"
#include <memory>

namespace c = containos;

namespace {

struct IntWrap
{
    IntWrap()                       { m_mem = new int(33); }
    IntWrap(int i)                  { m_mem = new int(i); }
    IntWrap(IntWrap const& other)   { m_mem = new int(*other.m_mem); }
    ~IntWrap()                      { delete m_mem; }
    operator int() const            { return *m_mem; }
    int& operator=(int i)           { (*m_mem) = i; return *m_mem; }
    bool operator==(int i) const    { return *m_mem == i; }
private:
    int* m_mem; 
};

}

template<> struct allow_memcpy<IntWrap> {
    static const bool allowed = false;
};

TEST_SUITE(List)
{
    TEST(Empty)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        EXPECT_EQUAL(list.size(), 0u);
        EXPECT_EQUAL(list.capasity(), 0u);
    }

    TEST(Reserve)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        EXPECT_EQUAL(list.size(), 0u);
        EXPECT_EQUAL(list.capasity(), 0u);
        list.reserve(10);
        EXPECT_EQUAL(list.size(), 0u);
        EXPECT_EQUAL(list.capasity(), 10u);
    }

    TEST(ReserveMore)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        EXPECT_EQUAL(list.size(), 0u);
        EXPECT_EQUAL(list.capasity(), 0u);
        list.reserve(1);
        list.insert(13);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list.capasity(), 1u);
        list.reserve(2);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list.capasity(), 2u);
        EXPECT_EQUAL(list[0], 13);
    }

    TEST(Copy)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(3);
        list.insert(3);
        list.insert(5);
        list.insert(15);
        c::List<IntWrap,c::ListGrowRule<0> > list2;
        list2.reserve(4);
        list2.insert(9);
        list2.insert(13);
        list2 = list;
        EXPECT_EQUAL(list2.size(), 3u);
        EXPECT_EQUAL(list2[0], 3);
        EXPECT_EQUAL(list2[1], 5);
        EXPECT_EQUAL(list2[2], 15);
    }

    TEST(Insert)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 3);
        list.insert(7);
        EXPECT_EQUAL(list.size(), 2u);
        EXPECT_EQUAL(list[1], 7);
    }

    TEST(InsertOther)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(3);
        list.insert(3);
        list.insert(5);
        c::List<IntWrap,c::ListGrowRule<0> > list2;
        list2.reserve(4);
        list2.insert(9);
        list2.insert(list);
        list2.insert(13);
        EXPECT_EQUAL(list2.size(), 4u);
        EXPECT_EQUAL(list2[0], 9);
        EXPECT_EQUAL(list2[1], 3);
        EXPECT_EQUAL(list2[2], 5);
        EXPECT_EQUAL(list2[3], 13);
    }

    TEST(Acquire)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.acquire() = 3;
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 3);
        list.acquire() = 7;
        EXPECT_EQUAL(list.size(), 2u);
        EXPECT_EQUAL(list[1], 7);
    }

    TEST(Remove)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        EXPECT_EQUAL(list.size(), 1u);
        list.remove(0);
        EXPECT_EQUAL(list.size(), 0u);
    }

    TEST(Pop)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        list.insert(7);
        EXPECT_EQUAL(list.size(), 2u);
        IntWrap result = 1;
        EXPECT_TRUE(list.pop(result));
        EXPECT_EQUAL(result, 7);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_TRUE(list.pop(result));
        EXPECT_EQUAL(result, 3);
        EXPECT_EQUAL(list.size(), 0u);
    }

    TEST(Clear)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(3);
        list.insert(3);
        list.insert(7);
        list.insert(4);
        EXPECT_EQUAL(list.size(), 3u);
        list.clear();
        EXPECT_EQUAL(list.size(), 0u);
    }

    TEST(ResizeToSmaller)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        list.insert(5);
        EXPECT_EQUAL(list.size(), 2u);
        list.resize(1);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 3);
    }

    TEST(ResizeToBigger)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        list.insert(5);
        EXPECT_EQUAL(list.size(), 2u);
        list.resize(3);
        EXPECT_EQUAL(list.size(), 3u);
        EXPECT_EQUAL(list[0], 3);
        EXPECT_EQUAL(list[1], 5);
        EXPECT_EQUAL(list[2], 33);
    }

    TEST(ResizeToZero)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        list.insert(5);
        EXPECT_EQUAL(list.size(), 2u);
        list.resize(0);
        EXPECT_EQUAL(list.size(), 0u);
    }

    TEST(ResizeNoCopy)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(2);
        list.insert(3);
        list.insert(5);
        EXPECT_EQUAL(list.size(), 2u);
        list.resizeNoCopy(4);
        EXPECT_EQUAL(list.size(), 4u);
        EXPECT_EQUAL(list[0], 33);
        EXPECT_EQUAL(list[1], 33);
        EXPECT_EQUAL(list[2], 33);
        EXPECT_EQUAL(list[3], 33);
    }

    TEST(ResizeWithIterator)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(5);
        list.insert(1);
        list.insert(2);
        list.insert(3);
        list.insert(4);
        EXPECT_EQUAL(list.size(), 4u);
        c::List<IntWrap,c::ListGrowRule<0> >::iterator it = list.begin();
        list.resize(++it);
        EXPECT_EQUAL(list.size(), 1u);
        EXPECT_EQUAL(list[0], 1);
    }

    TEST(Iterate)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(3);
        list.insert(0);
        list.insert(1);
        list.insert(2);
        c::List<IntWrap,c::ListGrowRule<0> >::iterator it = list.begin();
        c::List<IntWrap,c::ListGrowRule<0> >::iterator end = list.end();
        int i = 0;
        for( ; it != end; ++it, ++i ) {
            EXPECT_EQUAL(*it, i);
        }
        EXPECT_EQUAL(i, 3);
    }

    TEST(ConstIterate)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(3);
        list.insert(0);
        list.insert(1);
        list.insert(2);
        c::List<IntWrap,c::ListGrowRule<0> >::const_iterator it = list.begin();
        c::List<IntWrap,c::ListGrowRule<0> >::const_iterator end = list.end();
        int i = 0;
        for( ; it != end; ++it, ++i ) {
            EXPECT_EQUAL(*it, i);
        }
        EXPECT_EQUAL(i, 3);
    }

    TEST(DeleteIterator)
    {
        c::List<IntWrap,c::ListGrowRule<0> > list;
        list.reserve(3);
        list.insert(0);
        list.insert(1);
        list.insert(2);
        c::List<IntWrap>::iterator it = list.begin();
        ++it;
        list.remove(it);
        EXPECT_EQUAL(list.size(), 2u);
        EXPECT_EQUAL(list[0], 0);
        EXPECT_EQUAL(list[1], 2);
    }

    TEST(GrowRule)
    {
        c::List<IntWrap,c::ListGrowRule<8> > list;
        list.insert(0);
        list.insert(1);
        list.insert(2);
        EXPECT_EQUAL(list.size(), 3u);
        EXPECT_EQUAL(list.capasity(), 8u);
        EXPECT_EQUAL(list[0], 0);
        EXPECT_EQUAL(list[1], 1);
        EXPECT_EQUAL(list[2], 2);
    }
}
