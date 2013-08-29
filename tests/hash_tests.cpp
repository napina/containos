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
#include "containos/hash.h"

namespace c = containos;

TEST_SUITE(Hash)
{
    TEST(Zero32)
    {
        c::uint32_t compiled1 = c::hash32("", 0);
        c::uint32_t compiled2 = c::hash32("");
        EXPECT_EQUAL(compiled1, 0x811c9dc5);
        EXPECT_EQUAL(compiled2, 0x811c9dc5);
    }

    TEST(Compiled32)
    {
        const c::uint32_t compiled = c::hash32("Apina");
        EXPECT_EQUAL(compiled, 0x69c0613e);
    }

    TEST(Generic32)
    {
        const c::uint32_t compiled = c::hash32("Apina", 5);
        EXPECT_EQUAL(compiled, 0x69c0613e);
    }

    TEST(CompiledVsDynamic32)
    {
        const c::uint32_t compiled = c::hash32("Apina");
        char* dynamicStr = new char[6];
        ::memcpy(dynamicStr, "Apina", 5);
        dynamicStr[5] = 0;
        c::uint32_t dynamic = c::hash32(dynamicStr);
        EXPECT_EQUAL(compiled, dynamic);
        delete [] dynamicStr;
    }

    TEST(Zero64)
    {
        c::uint64_t compiled1 = c::hash64("", 0);
        c::uint64_t compiled2 = c::hash64("");
        EXPECT_EQUAL(compiled1, 0xcbf29ce484222325);
        EXPECT_EQUAL(compiled2, 0xcbf29ce484222325);
    }

    TEST(Compiled64)
    {
        const c::uint64_t compiled = c::hash64("Apina");
        EXPECT_EQUAL(compiled, 0x91b452b50d03fd5e);
    }

    TEST(Generic64)
    {
        const c::uint64_t compiled = c::hash64("Apina", 5);
        EXPECT_EQUAL(compiled, 0x91b452b50d03fd5e);
    }

    TEST(CompiledVsDynamic64)
    {
        const c::uint64_t compiled = c::hash64("Apina");
        char* dynamicStr = new char[6];
        ::memcpy(dynamicStr, "Apina", 5);
        dynamicStr[5] = 0;
        c::uint64_t dynamic = c::hash64(dynamicStr);
        EXPECT_EQUAL(compiled, dynamic);
        delete [] dynamicStr;
    }
}
