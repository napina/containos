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
#ifndef containos_container_h
#define containos_container_h

#include "containos/common.h"

/*----------------------------------------------------------------------------
struct Allocator
{
    static void* alloc(size_t size, size_t alignment);
    static void  dealloc(void* ptr);
};
//--------------------------------------------------------------------------*/

namespace containos {

namespace internal {
    template<typename Allocator, bool IsStatic> struct ContainerBase;
}

template<typename Allocator>
class Container : protected internal::ContainerBase<Allocator,std::is_pod<Allocator>::value>
{
    typedef internal::ContainerBase<Allocator,std::is_pod<Allocator>::value> Base;
protected:
    template<typename T>
    T* construct();
    template<typename T,typename A>
    T* construct(A a);
    template<typename T,typename A,typename B>
    T* construct(A a, B b);
    template<typename T,typename A,typename B,typename C>
    T* construct(A a, B b, C c);
    template<typename T,typename A,typename B,typename C,typename D>
    T* construct(A a, B b, C c, D d);
    template<typename T,typename A,typename B,typename C,typename D,typename E>
    T* construct(A a, B b, C c, D d, E e);
    template<typename T,typename A,typename B,typename C,typename D,typename E,typename F>
    T* construct(A a, B b, C c, D d, E e, F f);
    template<typename T,typename A,typename B,typename C,typename D,typename E,typename F,typename G>
    T* construct(A a, B b, C c, D d, E e, F f, G g);
    template<typename T,typename A,typename B,typename C,typename D,typename E,typename F,typename G,typename H>
    T* construct(A a, B b, C c, D d, E e, F f, G g, H h);
    template<typename T>
    void destruct(T* ptr);

    template<typename T>
    T* constructArray(size_t size);
    template<typename T>
    void destructArray(T* ptr, size_t size);
};

} // end of containos

#include "containos/container.inl"

#endif
