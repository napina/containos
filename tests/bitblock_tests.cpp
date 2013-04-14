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
#include "containos/bitblock.h"

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

TEST_SUITE(BitBlock)
{
    TEST(Empty)
    {
        c::BitBlock<IntWrap> block;
        EXPECT_EQUAL(block.size(), 0);
    }

    TEST(Insert)
    {
        c::BitBlock<IntWrap> block;
        size_t index = block.insert(3);
        EXPECT_EQUAL(block.size(), 1);
        EXPECT_EQUAL(block[index], 3);
        index = block.insert(7);
        EXPECT_EQUAL(block.size(), 2);
        EXPECT_EQUAL(block[index], 7);
    }

/*    TEST(InsertOther)
    {
        c::BitBlock<int> block;
        block.insert(3);
        block.insert(5);
        c::BitBlock<int> block2;
        block2.insert(9);
        block2.insert(block);
        block2.insert(13);
        EXPECT_EQUAL(block2.size(), 4);
        //EXPECT_EQUAL(block2[0], 9);
        //EXPECT_EQUAL(block2[1], 3);
        //EXPECT_EQUAL(block2[2], 5);
        //EXPECT_EQUAL(block2[3], 13);
    }*/

    TEST(Acquire)
    {
        c::BitBlock<IntWrap> block;
        size_t index;
        block.acquire(index) = 3;
        EXPECT_EQUAL(block.size(), 1);
        EXPECT_EQUAL(block[index], 3);
        block.acquire(index) = 7;
        EXPECT_EQUAL(block.size(), 2);
        EXPECT_EQUAL(block[index], 7);
    }

    TEST(Remove)
    {
        c::BitBlock<IntWrap> block;
        size_t index = block.insert(3);
        EXPECT_EQUAL(block.size(), 1);
        block.remove(index);
        EXPECT_EQUAL(block.size(), 0);
    }

    TEST(Clear)
    {
        c::BitBlock<IntWrap> block;
        block.insert(3);
        block.insert(7);
        block.insert(4);
        EXPECT_EQUAL(block.size(), 3);
        block.clear();
        EXPECT_EQUAL(block.size(), 0);
    }

    TEST(Iterate)
    {
        c::BitBlock<IntWrap> block;
        block.insert(0);
        block.insert(1);
        block.insert(2);
        c::BitBlock<IntWrap>::iterator it = block.begin();
        c::BitBlock<IntWrap>::iterator end = block.end();
        int i = 0;
        for( ; it != end; ++it, ++i ) {
            EXPECT_EQUAL(*it, i);
        }
        EXPECT_EQUAL(i, 3);
    }

    TEST(RemoveMiddleIterate)
    {
        c::BitBlock<IntWrap> block;
        block.insert(0);
        size_t index = block.insert(1);
        block.insert(2);
        block.remove(index);

        c::BitBlock<IntWrap>::iterator it = block.begin();
        c::BitBlock<IntWrap>::iterator end = block.end();
        EXPECT_EQUAL(*it, 0);
        EXPECT_NOTEQUAL(it, end);
        ++it;
        EXPECT_EQUAL(*it, 2);
        EXPECT_NOTEQUAL(it, end);
        ++it;
        EXPECT_EQUAL(it, end);
    }
}
