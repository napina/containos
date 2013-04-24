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
#ifndef containos_hash_h
#define containos_hash_h

#include "containos/config.h"

namespace containos {

// helper for function specialization
struct ConstCharWrapper {
    inline ConstCharWrapper(const char* str) : m_str(str) {}
    const char* m_str;
};

template<uint32_t Count> __forceinline uint32_t hash32(char const (&str)[Count]);
template<uint64_t Count> __forceinline uint64_t hash64(char const (&str)[Count]);

__forceinline uint32_t hash32(ConstCharWrapper str);
__forceinline uint64_t hash64(ConstCharWrapper str);

} // end of containos

#include "containos/hash.inl"

#endif
