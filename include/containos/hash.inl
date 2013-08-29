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
#ifndef containos_hash_inl
#define containos_hash_inl

namespace containos {

namespace internal {

// Based on "Quasi compile-time string hashing" by Stefan Reinalter
template<uint32_t Count,uint32_t Index>
struct Hasher32 {
    __forceinline static uint32_t generate(const char (&str)[Count]) {
        return (Hasher32<Count,Index - 1>::generate(str) ^ str[Index - 1]) * 16777619u;
    }
};

template<uint32_t Count>
struct Hasher32<Count,0> {
    __forceinline static uint32_t generate(const char (&)[Count]) {
        return 2166136261u;
    }
};

template<uint64_t Count,uint64_t Index>
struct Hasher64 {
    __forceinline static uint64_t generate(const char (&str)[Count]) {
        return (Hasher64<Count,Index - 1>::generate(str) ^ str[Index - 1]) * 1099511628211ull;
    }
};

template<uint64_t Count>
struct Hasher64<Count,0> {
    __forceinline static uint64_t generate(const char (&)[Count]) {
        return 14695981039346656037ull;
    }
};

} // end of internal
//-----------------------------------------------------------------------------

template<uint32_t Count>
__forceinline uint32_t hash32(char const (&str)[Count])
{
    return internal::Hasher32<Count,Count - 1>::generate(str);
}

template<uint64_t Count>
__forceinline uint64_t hash64(char const (&str)[Count])
{
    return internal::Hasher64<Count,Count - 1>::generate(str);
}
//-----------------------------------------------------------------------------

__forceinline uint32_t hash32(ConstCharWrapper wrapper)
{
    char const* str = wrapper.m_str;
    uint32_t value = 2166136261u; // basis
    while(*str != 0) {
        value ^= str[0];
        value *= 16777619u; // prime
        ++str;
    }
    return value;
}

__forceinline uint64_t hash64(ConstCharWrapper wrapper)
{
    char const* str = wrapper.m_str;
    uint64_t value = 14695981039346656037ull; // basis
    while(*str != 0) {
        value ^= str[0];
        value *= 1099511628211ull; // prime
        ++str;
    }
    return value;
}
//-----------------------------------------------------------------------------

__forceinline uint32_t hash32(void const* data, size_t size)
{
    char const* ptr = reinterpret_cast<char const*>(data);
    char const* ptrEnd = ptr + size;
    uint32_t value = 2166136261u; // basis
    while(ptr != ptrEnd) {
        value ^= ptr[0];
        value *= 16777619u; // prime
        ++ptr;
    }
    return value;
}

__forceinline uint64_t hash64(void const* data, size_t size)
{
    char const* ptr = reinterpret_cast<char const*>(data);
    char const* ptrEnd = ptr + size;
    uint64_t value = 14695981039346656037ull; // basis
    while(ptr != ptrEnd) {
        value ^= ptr[0];
        value *= 1099511628211ull; // prime
        ++ptr;
    }
    return value;
}

} // end of containos

#endif
