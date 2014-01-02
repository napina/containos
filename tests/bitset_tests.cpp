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
#include "containos/bitset.h"

namespace c = containos;
typedef c::uint32_t uint32_t;
typedef c::uint64_t uint64_t;

TEST_SUITE(BitSet)
{
    TEST(BitCountForward32)
    {
        EXPECT_EQUAL(c::bsf32(0xFFFFFFFF),0U);
        EXPECT_EQUAL(c::bsf32(0x00123100),8U);
        EXPECT_EQUAL(c::bsf32(0x200FF0F8),3U);
        EXPECT_EQUAL(c::bsf32(0x00100000),20U);
        EXPECT_EQUAL(c::bsf32(0x80000000),31U);
        EXPECT_EQUAL(c::bsf32(0x00000001),0U);
    }

    TEST(BitCountReverse32)
    {
        EXPECT_EQUAL(c::bsr32(0xFFFFFFFF),31u);
        EXPECT_EQUAL(c::bsr32(0x00123100),20u);
        EXPECT_EQUAL(c::bsr32(0x200FF0F8),29u);
        EXPECT_EQUAL(c::bsr32(0x00100000),20u);
        EXPECT_EQUAL(c::bsr32(0x80000000),31u);
        EXPECT_EQUAL(c::bsr32(0x00000001),0u);
    }

    TEST(CountTrailingZeroes32)
    {
        EXPECT_EQUAL(c::ctz32(0xFFFFFFFF),0u);
        EXPECT_EQUAL(c::ctz32(0x00123100),8u);
        EXPECT_EQUAL(c::ctz32(0x200FF0F8),3u);
        EXPECT_EQUAL(c::ctz32(0x00100000),20u);
        EXPECT_EQUAL(c::ctz32(0x80000000),31u);
        EXPECT_EQUAL(c::ctz32(0x00000001),0u);
    }

    TEST(CountLeadingZeroes32)
    {
        EXPECT_EQUAL(c::clz32(0xFFFFFFFF),0u);
        EXPECT_EQUAL(c::clz32(0x00123100),11u);
        EXPECT_EQUAL(c::clz32(0x200FF0F8),2u);
        EXPECT_EQUAL(c::clz32(0x00100000),11u);
        EXPECT_EQUAL(c::clz32(0x80000000),0u);
        EXPECT_EQUAL(c::clz32(0x00000001),31u);
    }

    TEST(Empty32)
    {
        c::bitset32 counter;
        EXPECT_EQUAL(c::bitset32::num_bits, 32);
        for(uint32_t i = 0; i < 32; ++i) {
            EXPECT_FALSE(counter.isSet(i));
        }
        EXPECT_TRUE(counter == 0);
        EXPECT_FALSE(counter != 0);
    }

    TEST(Acquire32)
    {
        c::bitset32 counter;
        for(uint32_t i = 0; i < 32; ++i) {
            uint32_t index = counter.acquire();
            EXPECT_EQUAL(index, i^31);
            EXPECT_EQUAL(counter.count(), i+1);
            EXPECT_TRUE(counter.isSet(index));
        }
    }

    TEST(Pop32)
    {
        c::bitset32 counter;
        counter.set(20);
        EXPECT_TRUE(counter.isSet(20));
        EXPECT_EQUAL(counter.count(), 1u);
        uint32_t index = counter.pop();
        EXPECT_EQUAL(index, 20u);
        EXPECT_EQUAL(counter.count(), 0u);
    }

    TEST(Set32)
    {
        c::bitset32 counter;
        counter.set(22);
        EXPECT_EQUAL(counter.count(), 1u);
        EXPECT_TRUE(counter.isSet(22));
        EXPECT_EQUAL(counter.mask(), 0x00400000u);
        EXPECT_TRUE(counter == 0x00400000u);
        EXPECT_TRUE(counter != 0u);
    }

    TEST(Remove32)
    {
        c::bitset32 counter;
        uint32_t index0 = counter.acquire();
        uint32_t index1 = counter.acquire();
        uint32_t index2 = counter.acquire();
        EXPECT_EQUAL(counter.count(), 3u);
        EXPECT_TRUE(counter.isSet(index0));
        EXPECT_TRUE(counter.isSet(index1));
        EXPECT_TRUE(counter.isSet(index2));
        counter.remove(index1);
        EXPECT_EQUAL(counter.count(), 2u);
        EXPECT_FALSE(counter.isSet(index1));
    }

    TEST(Mask32)
    {
        c::bitset32 counter;
        counter.set(4);
        counter.set(27);
        counter.set(10);
        EXPECT_EQUAL(counter.mask(), 0x08000410u);
    }

    TEST(Highest32)
    {
        c::bitset32 counter;
        counter.set(4);
        counter.set(27);
        counter.set(10);
        EXPECT_EQUAL(counter.highest(), 27u);
    }

#if defined(CONTAINOS_ARCH64)
    TEST(BitCountReverse64)
    {
        EXPECT_EQUAL(c::bsr64(0xFFFFFFFFFFFFFFFF),63u);
        EXPECT_EQUAL(c::bsr64(0x0012000000003100),52u);
        EXPECT_EQUAL(c::bsr64(0x2000000FF00000F8),61u);
        EXPECT_EQUAL(c::bsr64(0x0000001000000000),36u);
        EXPECT_EQUAL(c::bsr64(0x8000000000000000),63u);
        EXPECT_EQUAL(c::bsr64(0x0000000000000001),0u);
    }

    TEST(BitCountForward64)
    {
        EXPECT_EQUAL(c::bsf64(0xFFFFFFFFFFFFFFFF),0u);
        EXPECT_EQUAL(c::bsf64(0x0012000000003100),8u);
        EXPECT_EQUAL(c::bsf64(0x2000000FF00000F8),3u);
        EXPECT_EQUAL(c::bsf64(0x0000001000000000),36u);
        EXPECT_EQUAL(c::bsf64(0x8000000000000000),63u);
        EXPECT_EQUAL(c::bsf64(0x0000000000000001),0u);
    }

    TEST(CountTrailingZeroes64)
    {
        EXPECT_EQUAL(c::ctz64(0xFFFFFFFFFFFFFFFF),0u);
        EXPECT_EQUAL(c::ctz64(0x0012000000003100),8u);
        EXPECT_EQUAL(c::ctz64(0x2000000FF00000F8),3u);
        EXPECT_EQUAL(c::ctz64(0x0000001000000000),36u);
        EXPECT_EQUAL(c::ctz64(0x8000000000000000),63u);
        EXPECT_EQUAL(c::ctz64(0x0000000000000001),0u);
    }

    TEST(CountLeadingZeroes64)
    {
        EXPECT_EQUAL(c::clz64(0xFFFFFFFFFFFFFFFF),0u);
        EXPECT_EQUAL(c::clz64(0x0012000000003100),11u);
        EXPECT_EQUAL(c::clz64(0x2000000FF00000F8),2u);
        EXPECT_EQUAL(c::clz64(0x0000001000000000),27u);
        EXPECT_EQUAL(c::clz64(0x8000000000000000),0u);
        EXPECT_EQUAL(c::clz64(0x0000000000000001),63u);
    }

    TEST(Empty64)
    {
        c::bitset64 counter;
        EXPECT_EQUAL(c::bitset64::num_bits, 64);
        for(uint64_t i = 0; i < 64; ++i) {
            EXPECT_FALSE(counter.isSet(i));
        }
        EXPECT_TRUE(counter == 0);
        EXPECT_FALSE(counter != 0);
    }

    TEST(Acquire64)
    {
        c::bitset64 counter;
        for(uint64_t i = 0; i < 64; ++i) {
            uint64_t index = counter.acquire();
            EXPECT_EQUAL(index, i^63);
            EXPECT_EQUAL(counter.count(), i+1);
            EXPECT_TRUE(counter.isSet(index));
        }
    }

    TEST(Pop64)
    {
        c::bitset64 counter;
        counter.set(47);
        EXPECT_TRUE(counter.isSet(47));
        EXPECT_EQUAL(counter.count(), 1u);
        uint64_t index = counter.pop();
        EXPECT_EQUAL(index, 47u);
        EXPECT_EQUAL(counter.count(), 0u);
    }

    TEST(Set64)
    {
        c::bitset64 counter;
        counter.set(42);
        EXPECT_EQUAL(counter.count(), 1u);
        EXPECT_TRUE(counter.isSet(42));
        EXPECT_EQUAL(counter.mask(), 0x0000040000000000u);
    }

    TEST(Remove64)
    {
        c::bitset64 counter;
        uint64_t index0 = counter.acquire();
        uint64_t index1 = counter.acquire();
        uint64_t index2 = counter.acquire();
        EXPECT_EQUAL(counter.count(), 3u);
        EXPECT_TRUE(counter.isSet(index0));
        EXPECT_TRUE(counter.isSet(index1));
        EXPECT_TRUE(counter.isSet(index2));
        counter.remove(index1);
        EXPECT_EQUAL(counter.count(), 2u);
        EXPECT_FALSE(counter.isSet(index1));
    }

    TEST(Mask64)
    {
        c::bitset64 counter;
        counter.set(4);
        counter.set(47);
        counter.set(30);
        EXPECT_EQUAL(counter.mask(), 0x0000800040000010u);
    }

    TEST(Highest64)
    {
        c::bitset64 counter;
        counter.set(4);
        counter.set(47);
        counter.set(30);
        EXPECT_EQUAL(counter.highest(), 47u);
    }
#endif
}
