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

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
  #define NOMINMAX
#endif
#include <windows.h>

void* atomicCompareAndSwapPtr(void*& r, void* comp, void* v)
{
    return InterlockedCompareExchangePointer(&r, v, comp);
}

uint32_t atomicAssign32(uint32_t& r, uint32_t v)
{
    return InterlockedExchange((LONG*)&r, (LONG)v);
}

uint32_t atomicInc32(uint32_t& r)
{
    return (uint32_t)InterlockedIncrement((LONG*)&r);
}

uint32_t atomicDec32(uint32_t& r)
{
    return (uint32_t)InterlockedDecrement((LONG*)&r);
}

uint32_t atomicAdd32(uint32_t& r, uint32_t v)
{
    return InterlockedExchangeAdd((LONG*)&r, (LONG)v) + v;
}

uint32_t atomicSub32(uint32_t& r, uint32_t v)
{
    return InterlockedExchangeAdd((LONG*)&r, -(LONG)v) - v;
}

#if defined(CONTAINOS_ARCH64)

uint64_t atomicAssign64(uint64_t& r, uint64_t v)
{
    return InterlockedExchange64((LONGLONG*)&r, (LONGLONG)v);
}

uint64_t atomicInc64(uint64_t& r)
{
    return (uint64_t)InterlockedIncrement64((LONGLONG*)&r);
}

uint64_t atomicDec64(uint64_t& r)
{
    return (uint64_t)InterlockedDecrement64((LONGLONG*)&r);
}

uint64_t atomicAdd64(uint64_t& r, uint64_t v)
{
    return InterlockedExchangeAdd64((LONGLONG*)&r, (LONGLONG)v) + v;
}

uint64_t atomicSub64(uint64_t& r, uint64_t v)
{
    return InterlockedExchangeAdd64((LONGLONG*)&r, -(LONGLONG)v) - v;
}

#endif
#endif

} // end of containos

#endif
