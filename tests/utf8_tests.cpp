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
// http://en.wikipedia.org/wiki/UTF-8
// http://en.wikipedia.org/wiki/UTF-16
// http://en.wikipedia.org/wiki/UTF-32
// http://en.wikipedia.org/wiki/Unicode

namespace c = containos;

TEST_SUITE(Utf8)
{
    TEST(SetChar)
    {
        const char buffer[] = { 0xa2u, 0x24u, 0xffu, 0xc0u, 0 };
        c::Utf8 utf8(buffer);
        EXPECT_TRUE(utf8.isValid());
        EXPECT_EQUAL(utf8.length(), 4);
        EXPECT_EQUAL(utf8.dataCount(), 7);
        EXPECT_EQUAL(utf8, buffer);
    }

    TEST(SetWChar)
    {
        const wchar_t* buffer = L"a½-~€ib";
        c::Utf8 utf8(buffer);
        EXPECT_TRUE(utf8.isValid());
        EXPECT_EQUAL(utf8.length(), 7);
        EXPECT_EQUAL(utf8.dataCount(), 10);
        EXPECT_EQUAL(utf8, buffer);
    }
    
    TEST(SetUtf8)
    {
        const c::uint8_t buffer[] = { 0xf0u, 0xa4u, 0xadu, 0xa2u, 0x24u, 0xc2u, 0xa2u, 0xe2u, 0x82u, 0xacu, 0u };
        c::Utf8 utf8(buffer);
        EXPECT_TRUE(utf8.isValid());
        EXPECT_EQUAL(utf8.length(), 4);
        EXPECT_EQUAL(utf8.dataCount(), 10);
        c::Utf8::const_iterator it = utf8.begin();
        EXPECT_EQUAL(*it, 0x24b62u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0x24u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0xa2u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0x20acu);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(it, utf8.end());
        EXPECT_EQUAL(utf8, buffer);
    }

    TEST(SetUtf32)
    {
        const c::uint32_t buffer[] = { 0x10000u, 0xd800u, 0x10fffdu, 0xdc00u, 0x6c34u, 0x24u, 0xfdd0u, 0xffffu, 0u };
        c::Utf8 utf8(buffer);
        EXPECT_TRUE(utf8.isValid());
        EXPECT_EQUAL(utf8.length(), 6);
        EXPECT_EQUAL(utf8.dataCount(), 18);
        c::Utf8::const_iterator it = utf8.begin();
        EXPECT_EQUAL(*it, 0x10000u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0x10fffdu);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0x6c34u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0x24u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0xfdd0u);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(*it, 0xffffu);
        EXPECT_NOTEQUAL(it, utf8.end());
        ++it;
        EXPECT_EQUAL(it, utf8.end());
        EXPECT_EQUAL(utf8, buffer);
    }

    TEST(PlacementCopy)
    {
        c::Utf8 copy;
        {
            c::Utf8 utf8("this.should be-shared");
            copy = utf8;
            EXPECT_TRUE(utf8.isValid());
            EXPECT_TRUE(copy.isValid());
            EXPECT_EQUAL(utf8, "this.should be-shared");
            EXPECT_EQUAL(copy, "this.should be-shared");
        }
        EXPECT_TRUE(copy.isValid());
        EXPECT_EQUAL(copy, "this.should be-shared");
    }

    TEST(Replace)
    {
        c::Utf8 utf8("test\\replace/folder\\slashes");
        EXPECT_TRUE(utf8.isValid());
        EXPECT_NOTEQUAL(utf8.findFirst('\\'), utf8.end());
        utf8.replace('\\', '/');
        EXPECT_TRUE(utf8.isValid());
        EXPECT_EQUAL(utf8.findFirst('\\'), utf8.end());
        EXPECT_EQUAL(utf8, "test/replace/folder/slashes");
    }

    TEST(Fix)
    {
        const c::uint8_t buffer[] = { 0xf0u, 0xa4u, 0xadu, 0xa2u, 0xff, 0x24u, 0xc2u, 0xa2u, 0xf8u, 0xe2u, 0x82u, 0xacu, 0u };
        c::Utf8 utf8(buffer);
        EXPECT_FALSE(utf8.isValid());
        utf8.fix();
        EXPECT_TRUE(utf8.isValid());
        EXPECT_EQUAL(utf8.length(), 4);
        EXPECT_EQUAL(utf8.dataCount(), 10);
        c::Utf8::const_iterator it = utf8.begin();
        EXPECT_EQUAL(*it, 0x24b62u);
        ++it;
        EXPECT_EQUAL(*it, 0x24u);
        ++it;
        EXPECT_EQUAL(*it, 0xa2u);
        ++it;
        EXPECT_EQUAL(*it, 0x20acu);
        ++it;
        EXPECT_EQUAL(it, utf8.end());
        EXPECT_EQUAL(utf8, buffer);
    }

    TEST(FindFirstLast)
    {
        const c::uint32_t buffer[] = { 0x10000u, 0xd800u, 0x10fffdu, 0xdc00u, 0x6c34u, 0x10fffdu, 0xfdd0u, 0xffffu, 0u };
        c::Utf8 utf8(buffer);
        c::Utf8::const_iterator firstit = utf8.findFirst(0x10fffdu);
        EXPECT_NOTEQUAL(firstit, utf8.end());
        EXPECT_EQUAL(*firstit, 0x10fffdu);
        c::Utf8::const_iterator lastit = utf8.findLast(0x10fffdu);
        EXPECT_NOTEQUAL(lastit, utf8.end());
        EXPECT_NOTEQUAL(lastit, firstit);
        EXPECT_EQUAL(*lastit, 0x10fffdu);
    }

    TEST(FullSlice)
    {
        c::Utf8 utf8("this.is.test.string");
        c::Utf8Slice slice = utf8.slice();
        EXPECT_EQUAL(slice, "this.is.test.string");
    }

    TEST(Slice)
    {
        c::Utf8 utf8("this.is.test.string");
        c::Utf8::const_iterator first = utf8.findFirst('.');
        c::Utf8::const_iterator last = utf8.findLast('.');
        EXPECT_NOTEQUAL(first, utf8.end());
        EXPECT_NOTEQUAL(last, utf8.end());
        c::Utf8Slice slice = utf8.slice(first, last);
        EXPECT_EQUAL(slice, ".is.test");
    }
}
