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

bool Utf8Slice::operator==(char const* str) const
{
    uint8_t const* ptr = m_begin;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(ptr != m_end) {
        if(*str == 0)
            return false;
        if(decodeUtfCharacter(state, codepoint, *ptr++) == decodestate_accept) {
            const uint32_t test = uint8_t(*str++);
            if(test != codepoint)
                return false;
        } else if(state == decodestate_reject) {
            state = decodestate_accept;
        }
    }
    return *str == 0;
}

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
    if(m_buffer != nullptr && capasity <= m_buffer->m_capasity) {
        m_buffer->m_dataCount = 0;   
        return;
    }

    destruct();

    m_buffer = (Utf8::Buffer*)::malloc(sizeof(Utf8::Buffer) + sizeof(uint8_t) * capasity);
    m_buffer->m_refCount = 1;
    m_buffer->m_capasity = uint32_t(capasity);
    m_buffer->m_dataCount = 0;
    m_length = 0;
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

void Utf8::set(char const* str, size_t count)
{
    // TODO figure out better way to allocate storage
    reserve(count * 2);
    append(str, count);
}

void Utf8::set(uint8_t const* str)
{
    set(str, countUtfElements(str));
}

void Utf8::set(uint8_t const* str, size_t count)
{
    reserve(count);
    append(str, count);
}

void Utf8::set(uint16_t const* str)
{
    set(str, countUtfElements(str));
}

void Utf8::set(uint16_t const* str, size_t count)
{
    // TODO figure out better way to allocate storage
    reserve(count * 3);
    append(str, count);
}

void Utf8::set(uint32_t const* str)
{
    set(str, countUtfElements(str));
}

void Utf8::set(uint32_t const* str, size_t count)
{
    // TODO figure out better way to allocate storage
    reserve(count * 4);
    append(str, count);
}

void Utf8::append(uint32_t codepoint)
{
    uint8_t* ptr = m_buffer->m_data;
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
    m_buffer->m_dataCount += uint32_t(ptrdiff_t(ptr) - ptrdiff_t(m_buffer->m_data));
    ++m_length;
}

void Utf8::append(Utf8 const& other)
{
    if(other.m_buffer != nullptr)
        append(other.data(), other.dataCount());
}

void Utf8::append(char const* str)
{
    append(str, ::strlen(str));
}

void Utf8::append(char const* str, size_t count)
{
    uint8_t* ptr = m_buffer->m_data;
    for(size_t i = 0; i < count; ++i) {
        const uint8_t value = *str++;
        if(value < 0x80) {
            *ptr++ = value;
        } else {
            *ptr++ = 0xc0u | ((value >> 6) & 0x1fu);
            *ptr++ = 0x80u | (value & 0x3fu);
        }
    }
    *ptr = 0;
    m_buffer->m_dataCount += uint32_t(ptrdiff_t(ptr) - ptrdiff_t(m_buffer->m_data));
    m_length += count;
}

void Utf8::append(uint8_t const* str, size_t count)
{
    containos_memcpy(m_buffer->m_data + m_buffer->m_dataCount, str, count);
    m_buffer->m_dataCount += uint32_t(count);
    m_buffer->m_data[m_buffer->m_dataCount] = 0;
    m_length += countUtfLength(str);
}

void Utf8::append(uint16_t const* str, size_t count)
{
    uint16_t const* end = str + count;
    uint8_t* ptr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(str != end) {
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
    m_buffer->m_dataCount += uint32_t(ptrdiff_t(ptr) - ptrdiff_t(m_buffer->m_data));
}

void Utf8::append(uint32_t const* str, size_t count)
{
    uint32_t const* end = str + count;
    uint8_t* ptr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(str != end) {
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
    m_buffer->m_dataCount += uint32_t(ptrdiff_t(ptr) - ptrdiff_t(m_buffer->m_data));
}

Utf8::const_iterator Utf8::findFirst(uint32_t codepoint) const
{
    const_iterator it = begin();
    for(; it != end(); ++it) {
        if(*it == codepoint)
            return it;
    }
    return end();
}

Utf8::const_iterator Utf8::findFirst(const_iterator start, uint32_t codepoint) const
{
    const_iterator it = start;
    for(; it != end(); ++it) {
        if(*it == codepoint)
            return it;
    }
    return end();
}

Utf8::const_iterator Utf8::findLast(uint32_t codepoint) const
{
    const_iterator it = begin();
    const_iterator result = end();
    for(; it != end(); ++it) {
        if(*it == codepoint)
            result = it;
    }
    return result;
}

Utf8::const_iterator Utf8::findLast(const_iterator start, uint32_t codepoint) const
{
    const_iterator it = start;
    const_iterator result = end();
    for(; it != end(); ++it) {
        if(*it == codepoint)
            result = it;
    }
    return result;
}

void Utf8::replace(char from, char to)
{
    if(from > 0x7f || to > 0x7f)
        return;

    uint8_t* ptr = m_buffer->m_data;
    while(*ptr != 0) {
        if(*ptr == from)
            *ptr = to;
        ++ptr;
    }
}

void Utf8::trim(Utf8Slice const& slice)
{
    if(m_buffer == nullptr)
        return;

    if(slice.m_begin < m_buffer->m_data)
        return;

    if(slice.m_end > m_buffer->m_data + m_buffer->m_dataCount)
        return;

    const size_t newDataCount = countUtfElements(slice.m_begin, slice.m_end);
    if(slice.m_begin != m_buffer->m_data)
        ::memcpy(m_buffer->m_data, slice.m_begin, newDataCount);
    m_buffer->m_data[newDataCount] = 0;
    m_buffer->m_dataCount = uint32_t(newDataCount);
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
    m_buffer->m_dataCount = uint32_t(ptrdiff_t(writePtr) - ptrdiff_t(m_buffer->m_data));
}

bool Utf8::isValid() const
{
    return m_buffer != nullptr && isValidUtfString(m_buffer->m_data);
}

bool Utf8::operator==(char const* str) const
{
    uint8_t const* ptr = m_buffer->m_data;
    uint32_t state = decodestate_accept;
    uint32_t codepoint = 0;
    while(*ptr != 0) {
        if(*str == 0)
            return false;
        if(decodeUtfCharacter(state, codepoint, *ptr++) == decodestate_accept) {
            const uint32_t test = uint8_t(*str++);
            if(test != codepoint)
                return false;
        } else if(state == decodestate_reject) {
            state = decodestate_accept;
        }
    }
    return *str == 0;
}

bool Utf8::operator==(uint8_t const* str) const
{
    uint8_t const* aptr = m_buffer->m_data;
    uint8_t const* bptr = str;
    uint32_t astate = decodestate_accept;
    uint32_t bstate = decodestate_accept;
    uint32_t acodepoint = 0;
    uint32_t bcodepoint = 0;
    // decode this in top loop
    while(*aptr != 0) {
        if(decodeUtfCharacter(astate, acodepoint, *aptr++) == decodestate_accept) {
            // now we have this codepoint so decode from str
            while(*bptr != 0) {
                if(decodeUtfCharacter(bstate, bcodepoint, *bptr++) == decodestate_accept) {
                    if(acodepoint == bcodepoint)
                        break;
                    return false;
                } else if(bstate == decodestate_reject) {
                    bstate = decodestate_accept;
                }
            }
            if(*bptr != 0)
                continue;
            break;
        } else if(astate == decodestate_reject) {
            astate = decodestate_accept;
        }
    }
    return *aptr == 0 && *bptr == 0;
}

bool Utf8::operator==(uint16_t const* str) const
{
    uint8_t const* aptr = m_buffer->m_data;
    uint16_t const* bptr = str;
    uint32_t astate = decodestate_accept;
    uint32_t bstate = decodestate_accept;
    uint32_t acodepoint = 0;
    uint32_t bcodepoint = 0;
    // decode this in top loop
    while(*aptr != 0) {
        if(decodeUtfCharacter(astate, acodepoint, *aptr++) == decodestate_accept) {
            // now we have this codepoint so decode from str
            while(*bptr != 0) {
                if(decodeUtfCharacter(bstate, bcodepoint, *bptr++) == decodestate_accept) {
                    if(acodepoint == bcodepoint)
                        break;
                    return false;
                } else if(bstate == decodestate_reject) {
                    bstate = decodestate_accept;
                }
            }
            if(*bptr != 0)
                continue;
            break;
        } else if(astate == decodestate_reject) {
            astate = decodestate_accept;
        }
    }
    return *aptr == 0 && *bptr == 0;
}

bool Utf8::operator==(uint32_t const* str) const
{
    uint8_t const* aptr = m_buffer->m_data;
    uint32_t const* bptr = str;
    uint32_t astate = decodestate_accept;
    uint32_t bstate = decodestate_accept;
    uint32_t acodepoint = 0;
    uint32_t bcodepoint = 0;
    // decode this in top loop
    while(*aptr != 0) {
        if(decodeUtfCharacter(astate, acodepoint, *aptr++) == decodestate_accept) {
            // now we have this codepoint so decode from str
            while(*bptr != 0) {
                if(decodeUtfCharacter(bstate, bcodepoint, *bptr++) == decodestate_accept) {
                    if(acodepoint == bcodepoint)
                        break;
                    return false;
                } else if(bstate == decodestate_reject) {
                    bstate = decodestate_accept;
                }
            }
            if(*bptr != 0)
                continue;
            break;
        } else if(astate == decodestate_reject) {
            astate = decodestate_accept;
        }
    }
    return *aptr == 0 && *bptr == 0;
}

} // end of containos
