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
#ifndef containos_config_h
#define containos_config_h

#include <type_traits>
#include <new>

#if defined(__linux__) && defined(__ELF__)
#   define CONTAINOS_LINUX
#   define CONTAINOS_ARCH32
#elif defined(__APPLE__) && defined(__MACH__)
#   define CONTAINOS_MACOS
#   define CONTAINOS_ARCH32
#elif defined(_WIN64) || defined(_M_X64)
#   define CONTAINOS_WINDOWS
#   define CONTAINOS_ARCH64
#elif defined(_WIN32) || defined(_M_IX86)
#   define CONTAINOS_WINDOWS
#   define CONTAINOS_ARCH32
#endif

#if defined(__GNUC__)
#   define __forceinline inline __attribute__((always_inline))
#elif !defined(_MSC_VER)
#   define inline __forceinline
#endif

// no need of #include <stdint.h>
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#ifndef containos_assert
#define containos_assert(Test)
#endif

#define containos_cast(Type,Ptr)    static_cast<Type >(Ptr)
#define containos_delete(Ptr)       delete (Ptr)
//----------------------------------------------------------------------------

#define containos_placement_new(ptr,t)                  containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr)
#define containos_placement_new1(ptr,t,a)               containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a)
#define containos_placement_new2(ptr,t,a,b)             containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b)
#define containos_placement_new3(ptr,t,a,b,c)           containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b,c)
#define containos_placement_new4(ptr,t,a,b,c,d)         containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b,c,d)
#define containos_placement_new5(ptr,t,a,b,c,d,e)       containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b,c,d,e)
#define containos_placement_new6(ptr,t,a,b,c,d,e,f)     containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b,c,d,e,f)
#define containos_placement_new7(ptr,t,a,b,c,d,e,f,g)   containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b,c,d,e,f,g)
#define containos_placement_new8(ptr,t,a,b,c,d,e,f,g,h) containos::internal::placement_new<t,std::is_pod<t>::value>::create(ptr,a,b,c,d,e,f,g,h)
#define containos_placement_copy(ptr,t,other)           containos::internal::placement_new<t,std::is_pod<t>::value>::copy(ptr,other)
#define containos_placement_delete(ptr,t)               containos::internal::placement_delete<t,std::has_trivial_destructor<t>::value>::destroy(ptr)
//----------------------------------------------------------------------------

namespace containos {
namespace internal {

template<typename T, bool IsPod>
struct placement_new {
    static T* create(void* ptr)                                 { return static_cast<T*>(ptr); }
    static T* copy(void* ptr, T& other)                         { return &(*static_cast<T*>(ptr) = other); }
    static T* copy(void* ptr, T const& other)                   { return &(*static_cast<T*>(ptr) = other); }
};

template<typename T>
struct placement_new<T,false> {
    static T* create(void* ptr)                                 { return ::new (ptr) T(); }
    template<typename A>
    static T* create(void* ptr,A a)                             { return ::new (ptr) T(a); }
    template<typename A, typename B>
    static T* create(void* ptr,A a,B b)                         { return ::new (ptr) T(a,b); }
    template<typename A, typename B, typename C>
    static T* create(void* ptr,A a,B b,C c)                     { return ::new (ptr) T(a,b,c); }
    template<typename A, typename B, typename C, typename D>
    static T* create(void* ptr,A a,B b,C c,D d)                 { return ::new (ptr) T(a,b,c,d); }
    template<typename A, typename B, typename C, typename D, typename E>
    static T* create(void* ptr,A a,B b,C c,D d,E e)             { return ::new (ptr) T(a,b,c,d,e); }
    template<typename A, typename B, typename C, typename D, typename E, typename F>
    static T* create(void* ptr,A a,B b,C c,D d,E e,F f)         { return ::new (ptr) T(a,b,c,d,e,f); }
    template<typename A, typename B, typename C, typename D, typename E, typename F, typename G>
    static T* create(void* ptr,A a,B b,C c,D d,E e,F f,G g)     { return ::new (ptr) T(a,b,c,d,e,f,g); }
    template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
    static T* create(void* ptr,A a,B b,C c,D d,E e,F f,G g,H h) { return ::new (ptr) T(a,b,c,d,e,f,g,h); }

    static T* copy(void* ptr, T& other)                         { return ::new (ptr) T(other); }
    static T* copy(void* ptr, T const& other)                   { return ::new (ptr) T(other); }
};
//----------------------------------------------------------------------------

template<typename T, bool HasTrivialDestructor>
struct placement_delete {
    static void destroy(void*)                                  {}
};

template<typename T>
struct placement_delete<T,false> {
    static void destroy(void* ptr)                              { reinterpret_cast<T*>(ptr)->~T(); }
};

} // end of internal
} // end of containos

#endif
