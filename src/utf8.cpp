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

#include <malloc.h>

namespace containos {

void Utf8::reserve(size_t capasity)
{
    destruct();

    m_buffer = (Utf8::Buffer*)::malloc(sizeof(Utf8::Buffer) + sizeof(uint8_t) * capasity);
    m_buffer->m_refCount = 1;
    m_buffer->m_capasity = uint32_t(capasity);
}

inline void Utf8::destruct()
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

uint32_t Utf8::operator[](size_t index) const
{
    const_iterator it = begin();
    for(size_t i = 0; it != end(); ++it, ++i) {
        if(i == index)
            return *it;
    }
    return 0;
}

inline void Utf8::set(char const* str, size_t length)
{
    reserve(length);

    containos_memcpy(m_buffer->m_data, str, length);
}

void Utf8::set(uint8_t const* str, size_t length)
{
    length;
    str;
    /*
    // TODO figure out better way to allocate storage
    reserve(length);

    uint16_t* data = m_buffer->m_data;
    for(size_t i = 0; i < length; ++i) {
        const uint32_t ch = extractUtfCharacter(str);
        if(ch <= 0xffff) {
            *data++ = uint16_t(ch);
        } else {
            *data++ = uint16_t(0xd800 + (ch >> 10));
            *data++ = uint16_t(0xdc00 + (ch & 0x3ff));
        }
    }*/
}

void Utf8::set(uint16_t const* str, size_t byteCount)
{
    byteCount;
    str;
    /*reserve(byteCount);
    containos_memcpy(m_buffer->m_data, str, byteCount);
    */
}

void Utf8::set(uint32_t const* str, size_t length)
{
    str;
    length;
    /*
    // TODO figure out better way to allocate storage
    reserve(length * 2);

    uint16_t* data = m_buffer->m_data;
    for(size_t i = 0; i < length; ++i) {
        const uint32_t ch = *str++;
        if(ch <= 0xffff) {
            *data++ = uint16_t(ch);
        } else {
            *data++ = uint16_t(0xd800 + (ch >> 10));
            *data++ = uint16_t(0xdc00 + (ch & 0x3ff));
        }
    }*/
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

} // end of containos
