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
#pragma once
#ifndef containos_utfutils_inl
#define containos_utfutils_inl

namespace containos {

namespace internal {
    extern const uint8_t s_utf8decode[364];
}

__forceinline bool isUtfInRange(uint32_t codepoint, uint32_t lo, uint32_t hi)
{
    return (codepoint - lo) < (hi - lo + 1);
}

__forceinline bool isUtfSurrogate(uint32_t codepoint)
{
    return isUtfInRange(codepoint, 0xd800, 0xdfff);
}

__forceinline bool isUtfNoncharacter(uint32_t codepoint)
{
    return isUtfInRange(codepoint, 0xfdd0, 0xfdef);
}

__forceinline bool isUtfReserved(uint32_t codepoint)
{
    return (codepoint & 0xfffe) == 0xfffe;
}
__forceinline bool isUtfOutOfRange(uint32_t codepoint)
{
    return codepoint > 0x10ffff;
}

inline bool isInvalidUtfChar(uint32_t codepoint)
{
    return isUtfSurrogate(codepoint) || isUtfOutOfRange(codepoint);
}

inline bool isValidUtfChar(uint32_t codepoint)
{
    return !isUtfSurrogate(codepoint) && !isUtfOutOfRange(codepoint);
}

inline bool isPrivateUtfChar(uint32_t codepoint)
{
    return isUtfNoncharacter(codepoint) || isUtfReserved(codepoint);
}

__forceinline uint32_t decodeUtfCharacter(uint32_t& state, uint32_t& codepoint, uint8_t value)
{
    // TODO make branchless
    const uint32_t type = internal::s_utf8decode[value];
    if(state == decodestate_accept) {
        state = internal::s_utf8decode[256 + type];
        codepoint = ((0xff >> type) & (value));
    } else {
        state = internal::s_utf8decode[256 + state + type];
        codepoint = ((value & 0x3fu) | (codepoint << 6));
    }
    return state;
}

__forceinline uint32_t decodeUtfCharacter(uint32_t& state, uint32_t& codepoint, uint16_t value)
{
    // TODO figure out faster way to validate character
    if(state == decodestate_accept) {
        state = isUtfInRange(value, 0xd800u, 0xdfffu) ? decodestate_continue : decodestate_accept;
        state = isUtfInRange(value, 0xdc00u, 0xdfffu) ? decodestate_reject : state;
        codepoint = value;
    } else {
        state = isUtfInRange(value, 0xdc00u, 0xdffdu) ? decodestate_accept : decodestate_reject;
        codepoint = 0x10000u + ((codepoint - 0xd800u) << 10) | (value - 0xdc00u);
    }
    return state;
}

__forceinline uint32_t decodeUtfCharacter(uint32_t& state, uint32_t& codepoint, uint32_t value)
{
    // TODO figure out faster way to validate character
    codepoint = value;
    state = isValidUtfChar(codepoint) ? decodestate_accept : decodestate_reject;
    return state;
}

__forceinline size_t countUtfLength(uint32_t const* str, uint32_t const* end)
{
    return (ptrdiff_t(end) - ptrdiff_t(str)) / 4;
}

} // end of containos

#endif
