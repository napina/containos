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
#include "containos/atomic.h"

namespace c = containos;
typedef c::uint32_t uint32_t;
typedef c::uint64_t uint64_t;

TEST_SUITE(Atomic)
{
    TEST(CompareAndSwapPtr32)
    {
        void* ptr = (void*)0x12345678;
        void* r = c::atomicCompareAndSwapPtr(ptr, (void*)0x10101010, (void*)0x87654321);
        EXPECT_EQUAL(ptr, (void*)0x12345678);
        EXPECT_EQUAL(r, (void*)0x12345678);
        r = c::atomicCompareAndSwapPtr(ptr, (void*)0x12345678, (void*)0x87654321);
        EXPECT_EQUAL(ptr, (void*)0x87654321);
        EXPECT_EQUAL(r, (void*)0x12345678);
    }

    TEST(CompareAndSet32)
    {
        uint32_t v = 0x12345678;
        uint32_t r = c::atomicCompareAndSet32(v, 0x10101010, 0x87654321);
        EXPECT_EQUAL(v, 0x12345678u);
        EXPECT_EQUAL(r, 0x12345678u);
        r = c::atomicCompareAndSet32(v, 0x12345678, 0x87654321);
        EXPECT_EQUAL(v, 0x87654321u);
        EXPECT_EQUAL(r, 0x12345678u);
    }

    TEST(Assign32)
    {
        uint32_t v = 100;
        uint32_t r = c::atomicAssign32(v, 33);
        EXPECT_EQUAL(v, 33u);
        EXPECT_EQUAL(r, 100u);
    }

    TEST(Inc32)
    {
        uint32_t v = 100;
        uint32_t r = c::atomicInc32(v);
        EXPECT_EQUAL(v, 101u);
        EXPECT_EQUAL(r, 101u);
    }

    TEST(Dec32)
    {
        uint32_t v = 100;
        uint32_t r = c::atomicDec32(v);
        EXPECT_EQUAL(v, 99u);
        EXPECT_EQUAL(r, 99u);
    }

    TEST(Add32)
    {
        uint32_t v = 100;
        uint32_t r = c::atomicAdd32(v, 10);
        EXPECT_EQUAL(v, 110u);
        EXPECT_EQUAL(r, 110u);
    }

    TEST(Sub32)
    {
        uint32_t v = 100;
        uint32_t r = c::atomicSub32(v, 10);
        EXPECT_EQUAL(v, 90u);
        EXPECT_EQUAL(r, 90u);
    }

#if defined(CONTAINOS_ARCH64)
    TEST(CompareAndSwapPtr64)
    {
        void* ptr = (void*)0x12345678AB;
        void* r = c::atomicCompareAndSwapPtr(ptr, (void*)0x10101010, (void*)0x87654641AB);
        EXPECT_EQUAL(ptr, (void*)0x12345678AB);
        EXPECT_EQUAL(r, (void*)0x12345678AB);
        r = c::atomicCompareAndSwapPtr(ptr, (void*)0x12345678AB, (void*)0x87654641AB);
        EXPECT_EQUAL(ptr, (void*)0x87654641AB);
        EXPECT_EQUAL(r, (void*)0x12345678AB);
    }

    TEST(CompareAndSet64)
    {
        uint64_t v = 0x12345678AB;
        uint64_t r = c::atomicCompareAndSet64(v, 0x10101010, 0x87654641AB);
        EXPECT_EQUAL(v, 0x12345678ABu);
        EXPECT_EQUAL(r, 0x12345678ABu);
        r = c::atomicCompareAndSet64(v, 0x12345678AB, 0x87654641AB);
        EXPECT_EQUAL(v, 0x87654641ABu);
        EXPECT_EQUAL(r, 0x12345678ABu);
    }

    TEST(Assign64)
    {
        uint64_t v = 100;
        uint64_t r = c::atomicAssign64(v, 0x100000000);
        EXPECT_EQUAL(v, 0x100000000u);
        EXPECT_EQUAL(r, 100u);
    }

    TEST(Inc64)
    {
        uint64_t v = 0xffffffff;
        uint64_t r = c::atomicInc64(v);
        EXPECT_EQUAL(v, 0x100000000u);
        EXPECT_EQUAL(r, 0x100000000u);
    }

    TEST(Dec64)
    {
        uint64_t v = 0x100000000;
        uint64_t r = c::atomicDec64(v);
        EXPECT_EQUAL(v, 0xffffffffu);
        EXPECT_EQUAL(r, 0xffffffffu);
    }

    TEST(Add64)
    {
        uint64_t v = 0xffffffff;
        uint64_t r = c::atomicAdd64(v, 16);
        EXPECT_EQUAL(v, 0x10000000fu);
        EXPECT_EQUAL(r, 0x10000000fu);
    }

    TEST(Sub64)
    {
        uint64_t v = 0x100000000;
        uint64_t r = c::atomicSub64(v, 16);
        EXPECT_EQUAL(v, 0xfffffff0u);
        EXPECT_EQUAL(r, 0xfffffff0u);
    }
#endif
}
