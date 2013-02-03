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
#include "containos/bitblock.h"

namespace c = containos;

TEST_SUITE(BitBlock32)
{
    TEST(Empty)
    {
		c::BitBlock32<int> block;
        EXPECT_EQUAL(block.size(), 0);
        EXPECT_EQUAL(block.capasity(), 32);
    }

    TEST(Insert)
    {
        c::BitBlock32<int> block;
        block.insert(3);
        EXPECT_EQUAL(block.size(), 1);
        //EXPECT_EQUAL(block[0], 3);
        block.insert(7);
        EXPECT_EQUAL(block.size(), 2);
        //EXPECT_EQUAL(block[1], 7);
    }

    TEST(InsertOther)
    {
        c::BitBlock32<int> block;
        block.insert(3);
        block.insert(5);
        c::BitBlock32<int> block2;
        block2.insert(9);
        block2.insert(block);
        block2.insert(13);
        EXPECT_EQUAL(block2.size(), 4);
        //EXPECT_EQUAL(block2[0], 9);
        //EXPECT_EQUAL(block2[1], 3);
        //EXPECT_EQUAL(block2[2], 5);
        //EXPECT_EQUAL(block2[3], 13);
    }

    TEST(Acquire)
    {
        c::BitBlock32<int> block;
        block.acquire() = 3;
        EXPECT_EQUAL(block.size(), 1);
        //EXPECT_EQUAL(block[0], 3);
        block.acquire() = 7;
        EXPECT_EQUAL(block.size(), 2);
        //EXPECT_EQUAL(block[1], 7);
    }

    TEST(Remove)
    {
        c::BitBlock32<int> block;
        block.insert(3);
        EXPECT_EQUAL(block.size(), 1);
        block.remove(0);
        EXPECT_EQUAL(block.size(), 0);
    }

    TEST(Clear)
    {
        c::BitBlock32<int> block;
        block.insert(3);
        block.insert(7);
        block.insert(4);
        EXPECT_EQUAL(block.size(), 3);
        block.clear();
        EXPECT_EQUAL(block.size(), 0);
    }
}
