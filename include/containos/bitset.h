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
#ifndef containos_bitset_h
#define containos_bitset_h

#include "containos\config.h"

namespace containos {
//-----------------------------------------------------------------------------

uint32 bsr32(uint32 x);
uint32 bsf32(uint32 x);
uint32 ctz32(uint32 x);
uint32 clz32(uint32 x);
uint32 popcnt32(uint32 x);

struct bitset32
{
    enum { num_bits = 32 };
    enum { index_bits = 5 };

    bitset32();
    bitset32(const bitset32& other);

    uint32 acquire();
    uint32 pop();
    void set(uint32 index);
    void remove(uint32 index);
    void clear();
    bool isSet(uint32 index) const;
    uint32 count() const;
    uint32 highest() const;
    uint32 mask() const;

    bool operator==(uint32 other) const;
    bool operator==(bitset32 const& other) const;
    bool operator!=(uint32 other) const;
    bool operator!=(bitset32 const& other) const;

private:
    uint32 m_mask;
};
//-----------------------------------------------------------------------------

#ifdef CONTAINOS_ARCH64

uint32 bsr64(uint64 x);
uint32 bsf64(uint64 x);
uint32 ctz64(uint64 x);
uint32 clz64(uint64 x);
uint64 popcnt64(uint64 x);

struct bitset64
{
    enum { num_bits = 64 };
    enum { index_bits = 6 };

    bitset64();
    bitset64(const bitset64& other);

    uint64 acquire();
    uint64 pop();
    void set(uint64 index);
    void remove(uint64 index);
    void clear();
    bool isSet(uint64 index) const;
    uint64 count() const;
    uint64 highest() const;
    uint64 mask() const;

    bool operator==(uint64 mask) const;
    bool operator==(bitset64 const& other) const;
    bool operator!=(uint64 mask) const;
    bool operator!=(bitset64 const& other) const;

private:
    uint64 m_mask;
};
#endif
//-----------------------------------------------------------------------------

#if defined(CONTAINOS_ARCH64)
    typedef bitset64 bitset;
#else
    typedef bitset32 bitset;
#endif
//-----------------------------------------------------------------------------

} // end of containos

#include "containos/bitset.inl"

#endif
