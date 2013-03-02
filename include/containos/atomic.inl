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
#ifndef containos_atomic_inl
#define containos_atomic_inl

namespace containos {

#if defined(CONTAINOS_WINDOWS)
#include <intrin.h>

__forceinline void memoryBarrier()
{
    _mm_sfence();
}

__forceinline void prefetch_nta(void const* ptr)
{
    _mm_prefetch((char const*)ptr, _MM_HINT_NTA);
}

__forceinline void* atomicCompareAndSwapPtr(void*& r, void* comp, void* v)
{
#if defined(CONTAINOS_ARCH64)
    return _InterlockedCompareExchangePointer(&r, v, comp);
#else
    return (void*)(long*)_InterlockedCompareExchange((long volatile*)&r, (long)(long*)v, (long)(long*)comp);
#endif
}

__forceinline uint32 atomicAssign32(uint32& r, uint32 v)
{
    return _InterlockedExchange((long*)&r, (long)v);
}

__forceinline uint32 atomicInc32(uint32& r)
{
    return (uint32)_InterlockedIncrement((long*)&r);
}

__forceinline uint32 atomicDec32(uint32& r)
{
    return (uint32)_InterlockedDecrement((long*)&r);
}

__forceinline uint32 atomicAdd32(uint32& r, uint32 v)
{
    return _InterlockedExchangeAdd((long*)&r, (long)v) + v;
}

__forceinline uint32 atomicSub32(uint32& r, uint32 v)
{
    return _InterlockedExchangeAdd((long*)&r, -(long)v) - v;
}

__forceinline uint32 atomicCompareAndSet32(uint32& r, uint32 comp, uint32 v)
{
    return (uint32)_InterlockedCompareExchange((long volatile*)&r, v, comp);
}

#if defined(CONTAINOS_ARCH64)

__forceinline uint64 atomicAssign64(uint64& r, uint64 v)
{
    return _InterlockedExchange64((long long*)&r, (long long)v);
}

__forceinline uint64 atomicInc64(uint64& r)
{
    return (uint64)_InterlockedIncrement64((long long*)&r);
}

__forceinline uint64 atomicDec64(uint64& r)
{
    return (uint64)_InterlockedDecrement64((long long*)&r);
}

__forceinline uint64 atomicAdd64(uint64& r, uint64 v)
{
    return _InterlockedExchangeAdd64((long long*)&r, (long long)v) + v;
}

__forceinline uint64 atomicSub64(uint64& r, uint64 v)
{
    return _InterlockedExchangeAdd64((long long*)&r, -(long long)v) - v;
}

__forceinline uint64 atomicCompareAndSet64(uint64& r, uint64 comp, uint64 v)
{
    return (uint64)_InterlockedCompareExchange64((long long volatile*)&r, v, comp);
}

#endif

#endif

} // end of containos

#endif
