/*=============================================================================

Copyright (c) 2014 Ville Ruusutie

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
//#include "stdafx.h"
#include "containos/utfutils.h"

namespace containos {

namespace internal {
/*=============================================================================
This part includes utf8 code by Bjoern Hoehrmann

Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
=============================================================================*/

const uint8_t s_utf8decode[364] = {
    // The first part of the table maps bytes to character classes that
    // to reduce the size of the transition table and create bitmasks.
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
     7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
     8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

    // The second part is a transition table that maps a combination
    // of a state of the automaton and a character class to a state.
     0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
    12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
    12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
    12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
    12,36,12,12,12,12,12,12,12,12,12,12, 
};
/*=============================================================================
End of code by Bjoern Hoehrmann
=============================================================================*/

} // end of internal

bool isValidUtfString(uint8_t const* str)
{
    uint32_t state = decodestate_accept;
    while(*str != 0) {
        const uint32_t type = internal::s_utf8decode[*str++];
        state = internal::s_utf8decode[256 + state + type];
        if(state != decodestate_reject)
            continue;
        return false;
    }
    return state == decodestate_accept;
}

bool isValidUtfString(uint16_t const* str)
{
    while(*str != 0) {
        const uint32_t first = *str++;
        if(!isUtfInRange(first, 0xd800, 0xdfff))
            continue;
        // two elements
        const uint32_t second = *str++;
        if(second >= 0xdc00 && second <= 0xdffd)
            continue;
        return false;
    }
    return true;
}

bool isValidUtfString(uint32_t const* str)
{
    while(*str != 0) {
        if(isValidUtfChar(*str++))
            continue;
        return false;
    }
    return true;
}

size_t countUtfBytes(uint8_t const* str)
{
    size_t count = 0;
    while(*str++ != 0) {
        ++count;
    }
    return count;
}

size_t countUtfBytes(uint16_t const* str)
{
    size_t count = 0;
    while(*str++ != 0) {
        ++count;
    }
    return count * 2;
}

size_t countUtfBytes(uint32_t const* str)
{
    size_t count = 0;
    while(*str++ != 0) {
        ++count;
    }
    return count * 4;
}

size_t countUtfElements(uint8_t const* str)
{
    size_t count = 0;
    while(*str++ != 0) {
        ++count;
    }
    return count;
}

size_t countUtfElements(uint16_t const* str)
{
    size_t count = 0;
    while(*str++ != 0) {
        ++count;
    }
    return count;
}

size_t countUtfElements(uint32_t const* str)
{
    size_t count = 0;
    while(*str++ != 0) {
        ++count;
    }
    return count;
}

size_t countUtfLength(uint8_t const* str)
{
    size_t length = 0;
    uint32_t state = decodestate_accept;
    while(*str != 0) {
        const uint32_t type = internal::s_utf8decode[*str++];
        state = internal::s_utf8decode[256 + state + type];
        if(state == decodestate_accept) {
            ++length;
        }
    }
    return length;
}

size_t countUtfLength(uint8_t const* str, uint8_t const* end)
{
    size_t length = 0;
    uint32_t state = decodestate_accept;
    while(str != end) {
        const uint32_t type = internal::s_utf8decode[*str++];
        state = internal::s_utf8decode[256 + state + type];
        if(state == decodestate_accept) {
            ++length;
        }
    }
    return length;
}

size_t countUtfLength(uint16_t const* str)
{
    size_t length = 0;
    while(*str != 0) {
        ++length;
        const uint32_t first = *str++;
        if(!isUtfInRange(first, 0xd800, 0xdfff))
            continue;
        ++str;
    }
    return length;
}

size_t countUtfLength(uint32_t const* str)
{
    size_t length = 0;
    while(*str++ != 0) {
        ++length;
    }
    return length;
}

} // end of containos
