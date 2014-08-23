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
#include "containos/utfutils.h"

// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
// http://en.wikipedia.org/wiki/UTF-8
// http://en.wikipedia.org/wiki/UTF-16
// http://en.wikipedia.org/wiki/UTF-32
// http://en.wikipedia.org/wiki/Unicode

namespace c = containos;

TEST_SUITE(UtfUtils)
{
    TEST(ValidCharacter)
    {
        EXPECT_TRUE(c::isValidUtfChar(0));
        EXPECT_TRUE(c::isValidUtfChar(0xd7ff));
        EXPECT_TRUE(c::isValidUtfChar(0xe000));
        EXPECT_TRUE(c::isValidUtfChar(0xfdcf));
        EXPECT_TRUE(c::isValidUtfChar(0xfdf0));
        EXPECT_TRUE(c::isValidUtfChar(0xfffd));
        EXPECT_TRUE(c::isValidUtfChar(0x10000));
        EXPECT_TRUE(c::isValidUtfChar(0x10fffd));
    }

    TEST(InvalidCharacter)
    {
        for(c::uint32_t i = 0xd800; i <= 0xdfff; ++i) {
            EXPECT_TRUE(c::isInvalidUtfChar(i));
            EXPECT_FALSE(c::isValidUtfChar(i));
        }

        for(c::uint32_t i = 0xfdd0; i <= 0xfdef; ++i) {
            EXPECT_TRUE(c::isInvalidUtfChar(i));
            EXPECT_FALSE(c::isValidUtfChar(i));
        }

        for(c::uint32_t i = 0; i <= 0x10; ++i) {
            EXPECT_TRUE(c::isInvalidUtfChar((i << 16) | 0xfffe));
            EXPECT_TRUE(c::isInvalidUtfChar((i << 16) | 0xffff));
            EXPECT_FALSE(c::isValidUtfChar((i << 16) | 0xfffe));
            EXPECT_FALSE(c::isValidUtfChar((i << 16) | 0xffff));
        }

        EXPECT_TRUE(c::isInvalidUtfChar(0x110000));
    }

    TEST(ValidUtf8)
    {
        const c::uint8_t buffer[] = { 0xf0u, 0xa4u, 0xadu, 0xa2u, 0x24u, 0xc2u, 0xa2u, 0xe2u, 0x82u, 0xacu, 0 };
        EXPECT_TRUE(c::isValidUtfString(buffer));
        EXPECT_EQUAL(c::countUtfBytes(buffer), 10);
        EXPECT_EQUAL(c::countUtfLength(buffer), 4);
    }

    TEST(ValidUtf16)
    {
        const c::uint16_t buffer[] = { 0xd800u, 0xdc00u, 0xfeffu, 0x6c34u, 0xd834u, 0xdd1eu, 0 };
        EXPECT_TRUE(c::isValidUtfString(buffer));
        EXPECT_EQUAL(c::countUtfBytes(buffer), 12);
        EXPECT_EQUAL(c::countUtfLength(buffer), 4);
    }

    TEST(ValidUtf32)
    {
        const c::uint32_t buffer[] = { 0xd7ff, 0xe000, 0xfdcf, 0xfdf0, 0xfffd, 0x10000, 0x10fffd, 0 };
        EXPECT_TRUE(c::isValidUtfString(buffer));
        EXPECT_EQUAL(c::countUtfBytes(buffer), 28);
        EXPECT_EQUAL(c::countUtfLength(buffer), 7);
    }

    TEST(DecodeUtf8Characters)
    {
        const c::uint8_t buffer[] = { 0xf0u, 0xa4u, 0xadu, 0xa2u, 0x24u, 0xc2u, 0xa2u, 0xe2u, 0x82u, 0xacu, 0 };
        const c::uint8_t* str = buffer;
        c::uint32_t codepoint = 0;
        c::uint32_t state = c::decodestate_accept;
        EXPECT_NOTEQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_NOTEQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_NOTEQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x24b62u);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x24u);
        EXPECT_NOTEQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0xa2u);
        EXPECT_NOTEQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_NOTEQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x20acu);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0u);
    }

    TEST(DecodeUtf16Characters)
    {
        const c::uint16_t buffer[] = { 0xd800u, 0xdc00u, 0xfeffu, 0xdbabu, 0xff00u, 0x6c34u, 0xde00u, 0xd834u, 0xdd1eu, 0xdbffu, 0xdffdu, 0 };
        const c::uint16_t* str = buffer;
        c::uint32_t codepoint = 0;
        c::uint32_t state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_continue);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x10000u);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0xfeffu);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_continue);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_reject);
        state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x6c34u);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_reject);
        state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_continue);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x1d11eu);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_continue);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x10fffdu);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0u);
    }

    TEST(DecodeUtf32Characters)
    {
        const c::uint32_t buffer[] = { 0x10000u, 0xd800u, 0x10fffdu, 0xdc00u, 0x6c34u, 0x24u, 0xfdd0u, 0xffffu, 0 };
        const c::uint32_t* str = buffer;
        c::uint32_t codepoint = 0;
        c::uint32_t state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x10000u);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_reject);
        state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x10fffdu);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_reject);
        state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x6c34u);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0x24u);
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_reject);
        state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_reject);
        state = c::decodestate_accept;
        EXPECT_EQUAL(c::decodeUtfCharacter(state, codepoint, *str++), c::decodestate_accept);
        EXPECT_EQUAL(codepoint, 0u);
    }
}
