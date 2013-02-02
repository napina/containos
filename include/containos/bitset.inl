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
#ifndef containos_bitset_inl
#define containos_bitset_inl

namespace containos {

#ifdef __GNUC__
    __forceinline static uint32_t bsr32(uint32_t x) { return __builtin_clz(x) ^ 31; }
    __forceinline static uint32_t bsf32(uint32_t x) { return __builtin_ctz(x); }
    __forceinline static uint32_t ctz32(uint32_t x) { return __builtin_ctz(x); }
    __forceinline static uint32_t clz32(uint32_t x) { return __builtin_clz(x); }
    __forceinline static uint32_t popcnt32(uint32_t x) { return __builtin_popcount(x); }

    __forceinline static uint32_t bsr64(uint64_t x) { return __builtin_clzll(x) ^ 63; }
    __forceinline static uint32_t bsf64(uint64_t x) { return __builtin_ctzll(x); }
    __forceinline static uint32_t ctz64(uint64_t x) { return __builtin_ctzll(x); }
    __forceinline static uint32_t clz64(uint64_t x) { return __builtin_clzll(x); }
    __forceinline static uint32_t popcnt64(uint64_t x) { return __builtin_popcountll(x); }
#elif defined(CONTAINOS_WINDOWS)
#   include <intrin.h>
#   pragma intrinsic(_BitScanForward)
#   pragma intrinsic(_BitScanReverse)
    __forceinline static uint32_t bsr32(uint32_t x) { uint32_t r; _BitScanReverse(&r, x); return r; }
    __forceinline static uint32_t bsf32(uint32_t x) { uint32_t r; _BitScanForward(&r, x); return r; }
    __forceinline static uint32_t ctz32(uint32_t x) { uint32_t r; _BitScanForward(&r, x); return r; }
    __forceinline static uint32_t clz32(uint32_t x) { uint32_t r; _BitScanReverse(&r, x); return r ^ 31; }
    __forceinline static uint32_t popcnt32(uint32_t x) { return __popcnt(x); }
#ifdef CONTAINOS_ARCH64
#   pragma intrinsic(_BitScanForward64)
#   pragma intrinsic(_BitScanReverse64)
    __forceinline static uint32_t bsr64(uint64_t x) { uint32_t r; _BitScanReverse64(&r, x); return r; }
    __forceinline static uint32_t bsf64(uint64_t x) { uint32_t r; _BitScanForward64(&r, x); return r; }
    __forceinline static uint32_t ctz64(uint64_t x) { uint32_t r; _BitScanForward64(&r, x); return r; }
    __forceinline static uint32_t clz64(uint64_t x) { uint32_t r; _BitScanReverse64(&r, x); return r ^ 63; }
    __forceinline static uint32_t popcnt64(uint64_t x) { return __popcnt64(x); }
#endif
#endif
//-----------------------------------------------------------------------------

__forceinline bitset32::bitset32()
    : m_mask(0xffffffff)
{
}

__forceinline bitset32::bitset32(const bitset32& other)
    : m_mask(other.m_mask)
{
}

__forceinline int bitset32::acquire()
{
    containos_assert(m_mask != 0);
#if defined(CONTAINOS_NATIVE_BSR)
    int index = containos::bsr32(m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    int index = containos::clz32(m_mask) ^ 31;
#endif
    m_mask &= ~(1 << index);
    return index;
}

__forceinline void bitset32::remove(int index)
{
    containos_assert(index < 32);
    m_mask |= (1 << index);
}

__forceinline void bitset32::clear()
{
    m_mask = 0xffffffff;
}

__forceinline int bitset32::count() const
{
    return containos::popcnt32(~m_mask);
}

__forceinline bool bitset32::isSet(int index) const
{
    containos_assert(index < 32);
    return (m_mask & (1 << index)) == 0;
}
//-----------------------------------------------------------------------------

#if CONTAINOS_ARCH64
__forceinline bitset64::bitset64()
    : m_mask(0xffffffffffffffff)
{
}

__forceinline bitset64::bitset64(const bitset64& other)
    : m_mask(other.m_mask)
{
}

__forceinline int bitset64::acquire()
{
    containos_assert(m_mask != 0);
#ifdef CONTAINOS_NATIVE_BSR
    int index = containos::bsr64(m_mask);
#elif CONTAINOS_NATIVE_CLZ
    int index = containos::clz64(m_mask) ^ 63;
#endif
    m_mask &= ~(1 << index);
    return index;
}

__forceinline void bitset64::remove(int index)
{
    containos_assert(index < 64);
    m_mask |= (1ULL << index);
}

__forceinline void bitset64::clear()
{
    m_mask = 0xffffffffffffffff;
}

__forceinline int bitset64::count() const
{
    return containos::popcnt64(~m_mask);
}

__forceinline bool bitset64::isSet(int index) const
{
    return (m_mask & (1 << index)) == 0;
}
#endif
//-----------------------------------------------------------------------------

} // end of containos

#endif
