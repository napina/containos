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
#ifndef containos_bitset_inl
#define containos_bitset_inl

#if defined(CONTAINOS_WINDOWS)
#include <intrin.h>
#endif

namespace containos {

#if defined(__arm__)
#   define CONTAINOS_NATIVE_CLZ
#else
#   define CONTAINOS_NATIVE_BSR
#endif

#ifdef __GNUC__
    __forceinline uint32_t bsr32(uint32_t x) { return __builtin_clz(x) ^ 31; }
    __forceinline uint32_t bsf32(uint32_t x) { return __builtin_ctz(x); }
    __forceinline uint32_t ctz32(uint32_t x) { return __builtin_ctz(x); }
    __forceinline uint32_t clz32(uint32_t x) { return __builtin_clz(x); }
    __forceinline uint32_t popcnt32(uint32_t x) { return __builtin_popcount(x); }
#if defined(CONTAINOS_ARCH64)
    __forceinline uint32_t bsr64(uint64_t x) { return __builtin_clzll(x) ^ 63; }
    __forceinline uint32_t bsf64(uint64_t x) { return __builtin_ctzll(x); }
    __forceinline uint32_t ctz64(uint64_t x) { return __builtin_ctzll(x); }
    __forceinline uint32_t clz64(uint64_t x) { return __builtin_clzll(x); }
    __forceinline uint64_t popcnt64(uint64_t x) { return __builtin_popcountll(x); }
#endif
#elif defined(CONTAINOS_WINDOWS)
    __forceinline uint32_t bsr32(uint32_t x) { uint32_t r; ::_BitScanReverse((unsigned long*)&r, x); return r; }
    __forceinline uint32_t bsf32(uint32_t x) { uint32_t r; ::_BitScanForward((unsigned long*)&r, x); return r; }
    __forceinline uint32_t ctz32(uint32_t x) { uint32_t r; ::_BitScanForward((unsigned long*)&r, x); return r; }
    __forceinline uint32_t clz32(uint32_t x) { uint32_t r; ::_BitScanReverse((unsigned long*)&r, x); return r ^ 31; }
    __forceinline uint32_t popcnt32(uint32_t x) { return ::__popcnt(x); }
#if defined(CONTAINOS_ARCH64)
    __forceinline uint32_t bsr64(uint64_t x) { uint32_t r; ::_BitScanReverse64((unsigned long*)&r, x); return r; }
    __forceinline uint32_t bsf64(uint64_t x) { uint32_t r; ::_BitScanForward64((unsigned long*)&r, x); return r; }
    __forceinline uint32_t ctz64(uint64_t x) { uint32_t r; ::_BitScanForward64((unsigned long*)&r, x); return r; }
    __forceinline uint32_t clz64(uint64_t x) { uint32_t r; ::_BitScanReverse64((unsigned long*)&r, x); return r ^ 63; }
    __forceinline uint64_t popcnt64(uint64_t x) { return ::__popcnt64(x); }
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

__forceinline uint32_t bitset32::acquire()
{
    containos_assert(m_mask != 0);
#if defined(CONTAINOS_NATIVE_BSR)
    uint32_t index = containos::bsr32(m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    uint32_t index = containos::clz32(m_mask) ^ 31;
#endif
    m_mask &= ~(1U << index);
    return index;
}

__forceinline uint32_t bitset32::pop()
{
#if defined(CONTAINOS_NATIVE_BSR)
    uint32_t index =  containos::bsr32(~m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    uint32_t index =  containos::clz32(~m_mask) ^ 31;
#endif
    m_mask |= (1U << index);
    return index;
}

__forceinline void bitset32::set(uint32_t index)
{
    containos_assert(index < 64);
    m_mask &= ~(1U << index);
}

__forceinline void bitset32::remove(uint32_t index)
{
    containos_assert(index < 32);
    m_mask |= (1U << index);
}

__forceinline void bitset32::clear()
{
    m_mask = 0xffffffff;
}

__forceinline uint32_t bitset32::count() const
{
    return containos::popcnt32(~m_mask);
}

__forceinline uint32_t bitset32::highest() const
{
#if defined(CONTAINOS_NATIVE_BSR)
    return containos::bsr32(~m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    return containos::clz32(~m_mask) ^ 31;
#endif
}

__forceinline uint32_t bitset32::mask() const
{
    return ~m_mask;
}

__forceinline bool bitset32::isSet(uint32_t index) const
{
    containos_assert(index < 32);
    return (m_mask & (1U << index)) == 0;
}

__forceinline bool bitset32::operator==(uint32_t mask) const
{
    return ~m_mask == mask;
}

__forceinline bool bitset32::operator==(bitset32 const& other) const
{
    return m_mask == other.m_mask;
}

__forceinline bool bitset32::operator!=(uint32_t mask) const
{
    return ~m_mask != mask;
}

__forceinline bool bitset32::operator!=(bitset32 const& other) const
{
    return m_mask != other.m_mask;
}
//-----------------------------------------------------------------------------

#if defined(CONTAINOS_ARCH64)
__forceinline bitset64::bitset64()
    : m_mask(0xffffffffffffffff)
{
}

__forceinline bitset64::bitset64(const bitset64& other)
    : m_mask(other.m_mask)
{
}

__forceinline uint64_t bitset64::acquire()
{
    containos_assert(m_mask != 0);
#ifdef CONTAINOS_NATIVE_BSR
    uint64_t index = containos::bsr64(m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    uint64_t index = containos::clz64(m_mask) ^ 63;
#endif
    m_mask &= ~(1ULL << index);
    return index;
}

__forceinline uint64_t bitset64::pop()
{
#if defined(CONTAINOS_NATIVE_BSR)
    uint64_t index = containos::bsr64(~m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    uint64_t index = containos::clz64(~m_mask) ^ 63;
#endif
    m_mask |= (1ULL << index);
    return index;
}

__forceinline void bitset64::set(uint64_t index)
{
    containos_assert(index < 64);
    m_mask &= ~(1ULL << index);
}

__forceinline void bitset64::remove(uint64_t index)
{
    containos_assert(index < 64);
    m_mask |= (1ULL << index);
}

__forceinline void bitset64::clear()
{
    m_mask = 0xffffffffffffffff;
}

__forceinline bool bitset64::isSet(uint64_t index) const
{
    return (m_mask & (1ULL << index)) == 0;
}

__forceinline uint64_t bitset64::count() const
{
    return containos::popcnt64(~m_mask);
}

__forceinline uint64_t bitset64::highest() const
{
#if defined(CONTAINOS_NATIVE_BSR)
    return containos::bsr64(~m_mask);
#elif defined(CONTAINOS_NATIVE_CLZ)
    return containos::clz64(~m_mask) ^ 63;
#endif
}

__forceinline uint64_t bitset64::mask() const
{
    return ~m_mask;
}

__forceinline bool bitset64::operator==(uint64_t mask) const
{
    return ~m_mask == mask;
}

__forceinline bool bitset64::operator==(bitset64 const& other) const
{
    return m_mask == other.m_mask;
}

__forceinline bool bitset64::operator!=(uint64_t mask) const
{
    return ~m_mask != mask;
}

__forceinline bool bitset64::operator!=(bitset64 const& other) const
{
    return m_mask != other.m_mask;
}

#endif
//-----------------------------------------------------------------------------

} // end of containos

#endif
