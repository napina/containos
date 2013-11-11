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
#ifndef containos_utf8_h
#define containos_utf8_h

#include "containos/container.h"

namespace containos {

// Utf8 container
template<typename Allocator=Mallocator>
class Utf8 : protected Container<Allocator>
{
    typedef Container<Allocator> Base;
public:
    struct iterator {
        bool operator==(iterator const& other) const;
        bool operator!=(iterator const& other) const;
        char_t operator*() const;
        void operator++();
    };
    struct const_iterator {
        bool operator==(const_iterator const& other) const;
        bool operator!=(const_iterator const& other) const;
        char_t operator*() const;
        void operator++();
    };

    ~Utf8();
    Utf8();
    explicit Utf8(uint8_t const* str, size_t byteCount);
    explicit Utf8(uint16_t const* str, size_t byteCount);
    explicit Utf8(uint32_t const* str, size_t byteCount);
    explicit Utf8(wchar_t const* wstr, size_t length);

    template<typename Allocator2> Utf8(Utf8<Allocator2> const& other);
    template<typename Allocator2> void operator=(Utf8<Allocator2> const& other);
    void operator=(Utf8<Allocator> const& other);

    void set(uint8_t const* str, size_t byteCount);
    void set(uint16_t const* str, size_t byteCount);
    void set(uint32_t const* str, size_t byteCount);
    void set(wchar_t const* wstr, size_t length);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    char_t& operator[](size_t index);
    char_t operator[](size_t index) const;
    uint8_t const* mem() const;
    size_t size() const;

private:
    uint8_t* m_mem;
    size_t m_size;
};

} // end of containos

//#include "containos/utf8.inl"

#endif
