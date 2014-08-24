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
#include "containos/utf8.h"

// TODO get rid of this
#include <malloc.h>

namespace containos {

uint32_t Utf8::const_iterator::operator*() const
{
    uint8_t* ptr = m_ptr;
    uint32_t state = m_state;
    uint32_t codepoint = 0;
    while(*ptr != 0) {
        if(decodeUtfCharacter(state, codepoint, *ptr++) == decodestate_accept) {
            break;
        } else if(state == decodestate_reject) {
            break;
        }
    }
    return codepoint;
}

void Utf8::const_iterator::operator++()
{
    uint32_t state = decodestate_accept;
    while(*m_ptr != 0) {
        const uint32_t type = internal::s_utf8decode[*m_ptr++];
        state = internal::s_utf8decode[256 + state + type];
        if(state == decodestate_accept || state == decodestate_reject) {
            return;
        }
    }
}

void Utf8::reserve(size_t capasity)
{
    destruct();

    m_buffer = (Utf8::Buffer*)::malloc(sizeof(Utf8::Buffer) + sizeof(uint8_t) * capasity);
    m_buffer->m_refCount = 1;
    m_buffer->m_capasity = uint32_t(capasity);
}

void Utf8::destruct()
{
    if(m_buffer == nullptr)
        return;

    const size_t count = --(m_buffer->m_refCount);
    if(count > 0)
        return;

    ::free(m_buffer);
    m_buffer = nullptr;
    m_length = 0;
}

void Utf8::set(char const* str)
{
    set(str, ::strlen(str));
}

void Utf8::set(char const* str, size_t length)
{
    // TODO figure out better way to allocate storage
    reserve(length * 2);

    uint8_t* ptr = m_buffer->m_data;
    for(size_t i = 0; i < length; ++i) {
        const uint8_t value = *str++;
        if(value < 0x80) {
            *ptr++ = value;
        } else {
            *ptr++ = 0xc0u | ((value >> 6) & 0x1fu);
            *ptr++ = 0x80u | (value & 0x3fu);
        }
    }
    *ptr = 0;
    m_length = length;
}

void Utf8::set(uint8_t const* str)
{
    set(str, countUtfBytes(str));
}

void Utf8::set(uint8_t const* str, size_t count)
{
    reserve(count);

    containos_memcpy(m_buffer->m_data, str, count);
    m_buffer->m_data[count] = 0;
    m_length = countUtfLength(m_buffer->m_data);
}

void Utf8::set(uint16_t const* str)
{
    set(str, countUtfBytes(str) / 2);
}

void Utf8::set(uint16_t const* str, size_t count)
{
    // TODO figure out better way to allocate storage
    reserve(count * 3);
    m_length = 0;

    uint8_t* ptr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(*str != 0) {
        if(decodeUtfCharacter(state, codepoint, *str++) == decodestate_accept) {
            ++m_length;
            if(codepoint < 0x80u) {
                *ptr++ = uint8_t(codepoint);
            } else if(codepoint < 0x800u) {
                *ptr++ = uint8_t(0xc0u | ((codepoint >> 6) & 0x1fu));
                *ptr++ = uint8_t(0x80u | (codepoint & 0x3fu));
            } else if(codepoint < 0x10000u) {
                *ptr++ = uint8_t(0xe0u | ((codepoint >> 12) & 0x1fu));
                *ptr++ = uint8_t(0x80u | ((codepoint >> 6) & 0x3fu));
                *ptr++ = uint8_t(0x80u | (codepoint & 0x3fu));
            } else {
                *ptr++ = uint8_t(0xf0u | ((codepoint >> 18) & 0x1fu));
                *ptr++ = uint8_t(0x80u | ((codepoint >> 12) & 0x3fu));
                *ptr++ = uint8_t(0x80u | ((codepoint >> 6) & 0x3fu));
                *ptr++ = uint8_t(0x80u | (codepoint & 0x3fu));
            }
        } else if(state == decodestate_reject) {
            state = decodestate_accept;
        }
    }
    *ptr = 0;
}

void Utf8::set(uint32_t const* str)
{
    set(str, countUtfLength(str));
}

void Utf8::set(uint32_t const* str, size_t length)
{
    // TODO figure out better way to allocate storage
    reserve(length * 4);
    m_length = 0;

    uint8_t* ptr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(*str != 0) {
        if(decodeUtfCharacter(state, codepoint, *str++) == decodestate_accept) {
            ++m_length;
            if(codepoint < 0x80u) {
                *ptr++ = uint8_t(codepoint);
            } else if(codepoint < 0x800u) {
                *ptr++ = uint8_t(0xc0u | ((codepoint >> 6) & 0x1fu));
                *ptr++ = uint8_t(0x80u | (codepoint & 0x3fu));
            } else if(codepoint < 0x10000u) {
                *ptr++ = uint8_t(0xe0u | ((codepoint >> 12) & 0x1fu));
                *ptr++ = uint8_t(0x80u | ((codepoint >> 6) & 0x3fu));
                *ptr++ = uint8_t(0x80u | (codepoint & 0x3fu));
            } else {
                *ptr++ = uint8_t(0xf0u | ((codepoint >> 18) & 0x1fu));
                *ptr++ = uint8_t(0x80u | ((codepoint >> 12) & 0x3fu));
                *ptr++ = uint8_t(0x80u | ((codepoint >> 6) & 0x3fu));
                *ptr++ = uint8_t(0x80u | (codepoint & 0x3fu));
            }
        } else if(state == decodestate_reject) {
            state = decodestate_accept;
        }
    }
    *ptr = 0;
}

Utf8::const_iterator Utf8::findFirst(uint32_t ch) const
{
    const_iterator it = begin();
    for(; it != end(); ++it) {
        if(*it == ch)
            return it;
    }
    return end();
}

Utf8::const_iterator Utf8::findLast(uint32_t ch) const
{
    const_iterator it = begin();
    const_iterator result = end();
    for(; it != end(); ++it) {
        if(*it == ch)
            result = it;
    }
    return result;
}

Utf8 Utf8::substring(const_iterator begin, const_iterator end) const
{
    const size_t length = countUtfLength(begin.ptr(), end.ptr());
    return Utf8(begin.ptr(), length);
}

Utf8 Utf8::substring(const_iterator end) const
{
    Utf8 result;
    result.m_buffer = m_buffer;
    result.m_length = 0;
    if(m_buffer != nullptr) {
        ++(m_buffer->m_refCount);
        result.m_length = countUtfLength(m_buffer->m_data, end.ptr());;
    }
    return result;
}

void Utf8::replace(uint32_t from, uint8_t to)
{
    // TODO
    if(to > 0x7f)
        return;

    uint8_t* readPtr = m_buffer->m_data;
    uint8_t* writePtr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(*readPtr != 0) {
        if(decodeUtfCharacter(state, codepoint, *readPtr++) == decodestate_accept) {
            if(codepoint != from)
                continue;
            *writePtr++ = to;
        } else if(state != decodestate_reject) {
            writePtr++;
        } else {
            state = decodestate_accept;
        }
    }
    *writePtr = 0;
}

void Utf8::fix()
{
    if(m_buffer == nullptr)
        return;
    m_length = 0;

    uint8_t* readPtr = m_buffer->m_data;
    uint8_t* writePtr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    while(*readPtr != 0) {
        const uint32_t value = *readPtr++;
        const uint32_t type = internal::s_utf8decode[value];
        state = internal::s_utf8decode[256 + state + type];
        
        if(state == decodestate_accept) {
            *writePtr++ = uint8_t(value);
            ++m_length;
        } else if(state != decodestate_reject) {
            *writePtr++ = uint8_t(value);
        } else {
            state = decodestate_accept;
        }
    }
    *writePtr = 0;
}

bool Utf8::isValid() const
{
    return m_buffer != nullptr && isValidUtfString(m_buffer->m_data);
}

} // end of containos
