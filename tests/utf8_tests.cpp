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
#include "containos/utf8.h"

// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

namespace c = containos;

TEST_SUITE(Utf8)
{
    TEST(CountFirst)
    {
        c::char_t test[] = { 0x00000000, 0x00000080, 0x00000800, 0x00010000, 0x00200000, 0x04000000 };
        for(size_t i = 0; i < 6; ++i) {
            c::Utf8<> str(&test[i], 1);
            EXPECT_EQUAL(str.size(), i+1);
        }
    }

    TEST(CountLast)
    {
        c::char_t test[] = { 0x0000007F, 0x000007FF, 0x0000FFFF, 0x001FFFFF, 0x03FFFFFF, 0x7FFFFFFF };
        for(size_t i = 0; i < 6; ++i) {
            c::Utf8<> str(&test[i], 1);
            EXPECT_EQUAL(str.size(), i+1);
        }
    }

    TEST(Iterator)
    {
        c::char_t test[] = { 0x0008213, 0x00101392, 0x00002282, 0x00028C92 };
        c::Utf8<> str(test, 4);
        size_t i = 0;
        for(c::Utf8<>::iterator it = str.begin(); it != str.end(); ++it, ++i) {
            EXPECT_EQUAL(*it, test[i]);
        }
    }

    TEST(ConstIterator)
    {
        c::char_t test[] = { 0x0008213, 0x00101392, 0x00002282, 0x00028C92 };
        c::Utf8<> str(test, 4);
        size_t i = 0;
        for(c::Utf8<>::const_iterator it = str.begin(); it != str.end(); ++it, ++i) {
            EXPECT_EQUAL(*it, test[i]);
        }
    }

    TEST(Index)
    {
        c::char_t test[] = { 0x0008213, 0x00101392, 0x00002282, 0x00028C92 };
        c::Utf8<> str(test, 4);
        for(size_t i = 0; i < str.size(); ++i) {
            EXPECT_EQUAL(str[i], test[i]);
        }
    }
}
