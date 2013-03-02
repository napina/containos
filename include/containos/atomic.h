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
#ifndef containos_atomic_h
#define containos_atomic_h

#include "containos\config.h"

namespace containos {

// makes sure that every preceding store is globally visible before any subsequent store
void memoryBarrier();

// prefetch data for only short period of time
void prefetch_nta(void const* ptr);

// does: oldr = r, if r == comp then r = v, ret oldr
void* atomicCompareAndSwapPtr(void*& r, void* comp, void* v);

// does: oldr = r, r = v, ret oldr
uint32 atomicAssign32(uint32& r, uint32 v);
// does: ++r; ret r
uint32 atomicInc32(uint32& r);
// does: --r, ret r
uint32 atomicDec32(uint32& r);
// does: r += v, ret r
uint32 atomicAdd32(uint32& r, uint32 v);
// does: r -= v, ret r
uint32 atomicSub32(uint32& r, uint32 v);
// does: oldr = r, if r == comp then r = v, ret oldr
uint32 atomicCompareAndSet32(uint32& r, uint32 comp, uint32 v);

#if defined(CONTAINOS_ARCH64)
// does: oldr = r, r = v, ret oldr
uint64 atomicAssign64(uint64& r, uint64 v);
// does: ++r; ret r
uint64 atomicInc64(uint64& r);
// does: --r, ret r
uint64 atomicDec64(uint64& r);
// does: r += v, ret r
uint64 atomicAdd64(uint64& r, uint64 v);
// does: r -= v, ret r
uint64 atomicSub64(uint64& r, uint64 v);
// does: oldr = r, if r == comp then r = v, ret oldr
uint64 atomicCompareAndSet64(uint64& r, uint64 comp, uint64 v);
#endif

} // end of containos

#include "containos/atomic.inl"

#endif
