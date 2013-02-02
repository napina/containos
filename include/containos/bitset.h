/*=============================================================================

Copyright (c) 2007-2013 Ville Ruusutie

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

static uint32_t bsr32(uint32_t x);
static uint32_t bsf32(uint32_t x);
static uint32_t ctz32(uint32_t x);
static uint32_t clz32(uint32_t x);
static uint32_t popcnt32(uint32_t x);

struct bitset32
{
    bitset32();
    bitset32(const bitset32& other);

    int acquire();
    void remove(int index);
    void clear();
    int count() const;
    bool isSet(int index) const;

private:
    uint32_t m_mask;
};
//-----------------------------------------------------------------------------

#ifdef CONTAINOS_ARCH64

static uint32_t bsr64(uint64_t x);
static uint32_t bsf64(uint64_t x);
static uint32_t ctz64(uint64_t x);
static uint32_t clz64(uint64_t x);
static uint32_t popcnt64(uint64_t x);

struct bitset64
{
    bitset64();
    bitset64(const bitset64& other);

    int acquire();
    void remove(int index);
    void clear();
    int count() const;
    bool isSet(int index) const;

private:
    uint64_t m_mask;
};
#endif
//-----------------------------------------------------------------------------

} // end of containos

#include "containos/bitset.inl"

#endif
