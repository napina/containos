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
#ifndef containos_config_h
#define containos_config_h

#define containos_assert(Test)
#define containos_cast(Type,Ptr)    static_cast<Type >(Ptr)
#define containos_new(Type)         new Type
#define containos_delete(Ptr)       delete (Ptr)
//----------------------------------------------------------------------------

#define containos_placement_new(ptr,t)                  containos::internal::placement_new<t,__is_pod(t)>::create(ptr)
#define containos_placement_new1(ptr,t,a)               containos::internal::placement_new<t,__is_pod(t)>::create(ptr,a)
#define containos_placement_new2(ptr,t,a,b)             containos::internal::placement_new<t,__is_pod(t)>::create(ptr,a,b)
#define containos_placement_new3(ptr,t,a,b,c)           containos::internal::placement_new<t,__is_pod(t)>::create(ptr,a,b,c)
#define containos_placement_new4(ptr,t,a,b,c,d)         containos::internal::placement_new<t,__is_pod(t)>::create(ptr,a,b,c,d)
#define containos_placement_copy(ptr,t,other)           containos::internal::placement_new<t,__is_pod(t)>::copy(ptr,other)
#define containos_placement_delete(ptr,t)               containos::internal::placement_delete<t,__has_trivial_destructor(t)>::destroy(ptr)
//----------------------------------------------------------------------------

namespace containos {
namespace internal {

template<typename T, bool IsPod>
struct placement_new {
    static T* create(void* ptr)                                 { return static_cast<T*>(ptr); }
    static void copy(T* ptr, T const& other)                    { (*ptr) = other; }
};

template<typename T>
struct placement_new<T,false> {
    static T* create(void* ptr)                                 { return new (ptr) T(); }
    template<typename A>
    static T* create(void* ptr, A a)                            { return new (ptr) T(a); }
    template<typename A, typename B>
    static T* create(void* ptr, A a, B b)                       { return new (ptr) T(a,b); }
    template<typename A, typename B, typename C>
    static T* create(void* ptr, A a, B b, C c)                  { return new (ptr) T(a,b,c); }
    template<typename A, typename B, typename C, typename D>
    static T* create(void* ptr, A a, B b, C c, D d)             { return new (ptr) T(a,b,c,d); }

    static T* copy(void* ptr, T const& other)                   { return new (ptr) T(other); }
};
//----------------------------------------------------------------------------

template<typename T, bool HasTrivialDestructor>
struct placement_delete {
    static void destroy(T*)                                     {}
};

template<typename T>
struct placement_delete<T,false> {
    static void destroy(T* ptr)                                 { (ptr)->~T(); }
};

} // end of internal
} // end of containos

#endif
