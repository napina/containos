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
#ifndef containos_utfutils_h
#define containos_utfutils_h

#include "containos/common.h"

namespace containos {

bool isUtfInRange(uint32_t codepoint, uint32_t lo, uint32_t hi);
bool isUtfSurrogate(uint32_t codepoint);
bool isUtfNoncharacter(uint32_t codepoint);
bool isInvalidUtfChar(uint32_t codepoint);
bool isValidUtfChar(uint32_t codepoint);
bool isPrivateUtfChar(uint32_t codepoint);

bool isValidUtfString(uint8_t const* str);
bool isValidUtfString(uint16_t const* str);
bool isValidUtfString(uint32_t const* str);

size_t countUtfBytes(uint8_t const* str);
size_t countUtfBytes(uint8_t const* str, uint8_t const* end);
size_t countUtfBytes(uint16_t const* str);
size_t countUtfBytes(uint16_t const* str, uint16_t const* end);
size_t countUtfBytes(uint32_t const* str);
size_t countUtfBytes(uint32_t const* str, uint32_t const* end);

size_t countUtfElements(uint8_t const* str);
size_t countUtfElements(uint8_t const* str, uint8_t const* end);
size_t countUtfElements(uint16_t const* str);
size_t countUtfElements(uint16_t const* str, uint16_t const* end);
size_t countUtfElements(uint32_t const* str);
size_t countUtfElements(uint32_t const* str, uint32_t const* end);

size_t countUtfLength(uint8_t const* str);
size_t countUtfLength(uint8_t const* str, uint8_t const* end);
size_t countUtfLength(uint16_t const* str);
size_t countUtfLength(uint16_t const* str, uint16_t const* end);
size_t countUtfLength(uint32_t const* str);
size_t countUtfLength(uint32_t const* str, uint32_t const* end);

enum DecodeState : uint32_t {
    decodestate_accept = 0,
    decodestate_reject = 12,
    decodestate_continue = 24,
    // also all other values are continue
};
// run one step in decoding. returns new state. modifies state and codepoint
uint32_t decodeUtfCharacter(uint32_t& state, uint32_t& codepoint, uint8_t value);
uint32_t decodeUtfCharacter(uint32_t& state, uint32_t& codepoint, uint16_t value);
uint32_t decodeUtfCharacter(uint32_t& state, uint32_t& codepoint, uint32_t value);

} // end of containos

#include "containos/utfutils.inl"

#endif
