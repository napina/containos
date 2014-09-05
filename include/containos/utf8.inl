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
#ifndef containos_utf8_inl
#define containos_utf8_inl

#include "containos/utfutils.h"

namespace containos {

struct Utf8::Buffer
{
    uint32_t m_refCount;
    uint32_t m_capasity;
    uint32_t m_dataCount;
    uint8_t m_data[1];
};

inline Utf8::const_iterator::const_iterator(uint8_t* ptr)
    : m_ptr(ptr)
    , m_state(decodestate_accept)
{
}

__forceinline bool Utf8::const_iterator::operator==(const_iterator const& other) const
{
    return m_ptr == other.m_ptr;
}

__forceinline bool Utf8::const_iterator::operator!=(const_iterator const& other) const
{
    return m_ptr != other.m_ptr;
}

__forceinline uint8_t const* Utf8::const_iterator::ptr() const
{
    return m_ptr;
}

__forceinline bool Utf8::const_iterator::isRejected() const
{
    return m_state == decodestate_reject;
}

__forceinline Utf8::~Utf8()
{
    destruct();
}

__forceinline Utf8::Utf8()
{
    m_buffer = nullptr;
    m_length = 0;
}

__forceinline Utf8::Utf8(Utf8 const& other)
{
    m_buffer = other.m_buffer;
    m_length = other.m_length;
    if(m_buffer != nullptr)
        ++(m_buffer->m_refCount);
}

__forceinline Utf8::Utf8(Utf8Slice const& slice)
{
    m_buffer = nullptr;
    set(slice.m_begin, countUtfElements(slice.m_begin, slice.m_end));
}

__forceinline Utf8::Utf8(char const* str)
{
    m_buffer = nullptr;
    set(str);
}

__forceinline Utf8::Utf8(char const* str, size_t count)
{
    m_buffer = nullptr;
    set(str, count);
}

__forceinline Utf8::Utf8(wchar_t const* str)
{
    m_buffer = nullptr;
    set(str);
}

__forceinline Utf8::Utf8(wchar_t const* str, size_t count)
{
    m_buffer = nullptr;
    set(str, count);
}

__forceinline Utf8::Utf8(uint8_t const* str)
{
    m_buffer = nullptr;
    set(str);
}

__forceinline Utf8::Utf8(uint8_t const* str, size_t count)
{
    m_buffer = nullptr;
    set(str, count);
}

__forceinline Utf8::Utf8(uint16_t const* str)
{
    m_buffer = nullptr;
    set(str);
}

__forceinline Utf8::Utf8(uint16_t const* str, size_t count)
{
    m_buffer = nullptr;
    set(str, count);
}

__forceinline Utf8::Utf8(uint32_t const* str)
{
    m_buffer = nullptr;
    set(str);
}

__forceinline Utf8::Utf8(uint32_t const* str, size_t count)
{
    m_buffer = nullptr;
    set(str, count);
}

template<size_t Count> __forceinline Utf8::Utf8(char const (&str)[Count])
{
    m_buffer = nullptr;
    set(str, Count);
}

template<size_t Count> __forceinline Utf8::Utf8(wchar_t const (&str)[Count])
{
    m_buffer = nullptr;
    set(str, Count);
}

__forceinline void Utf8::set(Utf8 const& other)
{
    destruct();
    m_buffer = other.m_buffer;
    m_length = other.m_length;
    if(m_buffer != nullptr)
        ++(m_buffer->m_refCount);
}

__forceinline void Utf8::set(Utf8Slice const& slice)
{
    set(slice.m_begin, countUtfElements(slice.m_begin, slice.m_end));
}

__forceinline void Utf8::set(wchar_t const* str)
{
    set(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str));
}

__forceinline void Utf8::set(wchar_t const* str, size_t count)
{
    set(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str), count);
}

template<size_t Count> __forceinline void Utf8::set(char const (&str)[Count])
{
    set(str, Count);
}

template<size_t Count> __forceinline void Utf8::set(wchar_t const (&str)[Count])
{
    set(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str), Count);
}

__forceinline void Utf8::append(uint8_t const* str)
{
    append(str, countUtfElements(str));
}

__forceinline void Utf8::append(uint16_t const* str)
{
    append(str, countUtfElements(str));
}

__forceinline void Utf8::append(uint32_t const* str)
{
    append(str, countUtfElements(str));
}

__forceinline void Utf8::append(wchar_t const* str)
{
    append(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str));
}

__forceinline void Utf8::append(wchar_t const* str, size_t count)
{
    append(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str), count);
}

template<size_t Count> __forceinline void Utf8::append(char const (&str)[Count])
{
    append(str, Count);
}

template<size_t Count> __forceinline void Utf8::append(wchar_t const (&str)[Count])
{
    append(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str), Count);
}

__forceinline void Utf8::convertTo(wchar_t* buffer, size_t count) const
{
    convertTo(reinterpret_cast<CONTAINOS_WCHAR_IS*>(buffer), count);
}

__forceinline Utf8::const_iterator Utf8::begin() const
{
    return const_iterator(m_buffer->m_data);
}

__forceinline Utf8::const_iterator Utf8::end() const
{
    return const_iterator(m_buffer->m_data + m_buffer->m_dataCount);
}

__forceinline Utf8Slice Utf8::slice(const_iterator begin, const_iterator end) const
{
    Utf8Slice result;
    result.m_begin = begin.ptr();
    result.m_end = end.ptr();
    return result;
}

__forceinline Utf8Slice Utf8::slice(const_iterator end) const
{
    Utf8Slice result;
    result.m_begin = m_buffer->m_data;
    result.m_end = end.ptr();
    return result;
}

__forceinline Utf8Slice Utf8::slice() const
{
    Utf8Slice result;
    result.m_begin = m_buffer->m_data;
    result.m_end = m_buffer->m_data + m_buffer->m_dataCount;
    return result;
}

__forceinline uint8_t const* Utf8::data() const
{
    if(m_buffer != nullptr)
        return m_buffer->m_data;
    return nullptr;
}

__forceinline size_t Utf8::dataCount() const
{
    return m_buffer->m_dataCount;
}

__forceinline size_t Utf8::capasity() const
{
    return m_buffer->m_capasity;
}

__forceinline size_t Utf8::length() const
{
    return m_length;
}

__forceinline void Utf8::operator=(Utf8 const& other)
{
    set(other);
}

__forceinline void Utf8::operator=(Utf8Slice const& slice)
{
    set(slice);
}

__forceinline bool Utf8::operator==(Utf8 const& other) const
{
    return operator==(other.data());
}

__forceinline bool Utf8::operator==(wchar_t const* str) const
{
    return operator==(reinterpret_cast<CONTAINOS_WCHAR_IS const*>(str));
}

} // end of containos

#endif
