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
#include "containos/bitset.h"

namespace c = containos;

TEST_SUITE(BitSet)
{
    TEST(BitCountForward32)
    {
        EXPECT_EQUAL(c::bsf32(0xFFFFFFFF),0);
        EXPECT_EQUAL(c::bsf32(0x00123100),8);
        EXPECT_EQUAL(c::bsf32(0x200FF0F8),3);
        EXPECT_EQUAL(c::bsf32(0x00100000),20);
        EXPECT_EQUAL(c::bsf32(0x80000000),31);
        EXPECT_EQUAL(c::bsf32(0x00000001),0);
    }

    TEST(BitCountReverse32)
    {
        EXPECT_EQUAL(c::bsr32(0xFFFFFFFF),31);
        EXPECT_EQUAL(c::bsr32(0x00123100),20);
        EXPECT_EQUAL(c::bsr32(0x200FF0F8),29);
        EXPECT_EQUAL(c::bsr32(0x00100000),20);
        EXPECT_EQUAL(c::bsr32(0x80000000),31);
        EXPECT_EQUAL(c::bsr32(0x00000001),0);
    }

    TEST(CountTrailingZeroes32)
    {
        EXPECT_EQUAL(c::ctz32(0xFFFFFFFF),0);
        EXPECT_EQUAL(c::ctz32(0x00123100),8);
        EXPECT_EQUAL(c::ctz32(0x200FF0F8),3);
        EXPECT_EQUAL(c::ctz32(0x00100000),20);
        EXPECT_EQUAL(c::ctz32(0x80000000),31);
        EXPECT_EQUAL(c::ctz32(0x00000001),0);
    }

    TEST(CountLeadingZeroes32)
    {
        EXPECT_EQUAL(c::clz32(0xFFFFFFFF),0);
        EXPECT_EQUAL(c::clz32(0x00123100),11);
        EXPECT_EQUAL(c::clz32(0x200FF0F8),2);
        EXPECT_EQUAL(c::clz32(0x00100000),11);
        EXPECT_EQUAL(c::clz32(0x80000000),0);
        EXPECT_EQUAL(c::clz32(0x00000001),31);
    }

    TEST(Empty32)
    {
        c::bitset32 counter;
        EXPECT_EQUAL(c::bitset32::num_bits, 32);
        for(uint32_t i = 0; i < 32; ++i) {
            EXPECT_FALSE(counter.isSet(i));
        }
    }

    TEST(Acquire32)
    {
        c::bitset32 counter;
        for(int i = 0; i < 32; ++i) {
            int index = counter.acquire();
            EXPECT_EQUAL(index, i^31);
            EXPECT_EQUAL(counter.count(), i+1);
            EXPECT_TRUE(counter.isSet(index));
        }
    }

	TEST(Set32)
    {
        c::bitset32 counter;
		counter.set(22);
        EXPECT_EQUAL(counter.count(), 1);
        EXPECT_TRUE(counter.isSet(22));
		EXPECT_EQUAL(counter.mask(), 0x00400000);
    }

    TEST(Remove32)
    {
        c::bitset32 counter;
        int index0 = counter.acquire();
        int index1 = counter.acquire();
        int index3 = counter.acquire();
        EXPECT_EQUAL(counter.count(), 3);
        EXPECT_TRUE(counter.isSet(index1));
        counter.remove(index1);
        EXPECT_EQUAL(counter.count(), 2);
        EXPECT_FALSE(counter.isSet(index1));
    }

	TEST(Mask32)
    {
        c::bitset32 counter;
		counter.set(4);
		counter.set(27);
		counter.set(10);
		EXPECT_EQUAL(counter.mask(), 0x08000410);
    }

	TEST(Highest32)
    {
        c::bitset32 counter;
		counter.set(4);
		counter.set(27);
		counter.set(10);
		EXPECT_EQUAL(counter.highest(), 27);
    }

#if defined(CONTAINOS_ARCH64)
    TEST(BitCountReverse64)
    {
        EXPECT_EQUAL(c::bsr64(0xFFFFFFFFFFFFFFFF),63);
        EXPECT_EQUAL(c::bsr64(0x0012000000003100),52);
        EXPECT_EQUAL(c::bsr64(0x2000000FF00000F8),61);
        EXPECT_EQUAL(c::bsr64(0x0000001000000000),36);
        EXPECT_EQUAL(c::bsr64(0x8000000000000000),63);
        EXPECT_EQUAL(c::bsr64(0x0000000000000001),0);
    }

    TEST(BitCountForward64)
    {
        EXPECT_EQUAL(c::bsf64(0xFFFFFFFFFFFFFFFF),0);
        EXPECT_EQUAL(c::bsf64(0x0012000000003100),8);
        EXPECT_EQUAL(c::bsf64(0x2000000FF00000F8),3);
        EXPECT_EQUAL(c::bsf64(0x0000001000000000),36);
        EXPECT_EQUAL(c::bsf64(0x8000000000000000),63);
        EXPECT_EQUAL(c::bsf64(0x0000000000000001),0);
    }

    TEST(CountTrailingZeroes64)
    {
        EXPECT_EQUAL(c::ctz64(0xFFFFFFFFFFFFFFFF),0);
        EXPECT_EQUAL(c::ctz64(0x0012000000003100),8);
        EXPECT_EQUAL(c::ctz64(0x2000000FF00000F8),3);
        EXPECT_EQUAL(c::ctz64(0x0000001000000000),36);
        EXPECT_EQUAL(c::ctz64(0x8000000000000000),63);
        EXPECT_EQUAL(c::ctz64(0x0000000000000001),0);
    }

    TEST(CountLeadingZeroes64)
    {
        EXPECT_EQUAL(c::clz64(0xFFFFFFFFFFFFFFFF),0);
        EXPECT_EQUAL(c::clz64(0x0012000000003100),11);
        EXPECT_EQUAL(c::clz64(0x2000000FF00000F8),2);
        EXPECT_EQUAL(c::clz64(0x0000001000000000),27);
        EXPECT_EQUAL(c::clz64(0x8000000000000000),0);
        EXPECT_EQUAL(c::clz64(0x0000000000000001),63);
    }

    TEST(Empty64)
    {
        c::bitset64 counter;
        EXPECT_EQUAL(c::bitset64::num_bits, 64);
        for(uint64_t i = 0; i < 64; ++i) {
            EXPECT_FALSE(counter.isSet(i));
        }
    }

    TEST(Acquire64)
    {
        c::bitset64 counter;
        for(int i = 0; i < 64; ++i) {
            uint64_t index = counter.acquire();
            EXPECT_EQUAL(index, i^63);
            EXPECT_EQUAL(counter.count(), i+1);
            EXPECT_TRUE(counter.isSet(index));
        }
    }

	TEST(Set64)
    {
        c::bitset64 counter;
		counter.set(42);
        EXPECT_EQUAL(counter.count(), 1);
        EXPECT_TRUE(counter.isSet(42));
		EXPECT_EQUAL(counter.mask(), 0x0000040000000000);
    }

    TEST(Remove64)
    {
        c::bitset64 counter;
        uint64_t index0 = counter.acquire();
        uint64_t index1 = counter.acquire();
        uint64_t index3 = counter.acquire();
        EXPECT_EQUAL(counter.count(), 3);
        EXPECT_TRUE(counter.isSet(index1));
        counter.remove(index1);
        EXPECT_EQUAL(counter.count(), 2);
        EXPECT_FALSE(counter.isSet(index1));
    }

	TEST(Mask64)
    {
        c::bitset64 counter;
		counter.set(4);
		counter.set(47);
		counter.set(30);
		EXPECT_EQUAL(counter.mask(), 0x0000800040000010);
    }

	TEST(Highest64)
    {
        c::bitset64 counter;
		counter.set(4);
		counter.set(47);
		counter.set(30);
		EXPECT_EQUAL(counter.highest(), 47);
    }
#endif
}
