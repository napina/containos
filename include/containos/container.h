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
#ifndef containos_container_h
#define containos_container_h

#include "containos/containerbase.h"

namespace containos {

template<typename Allocator>
class Container : protected internal::ContainerBase<Allocator, Allocator::IsStatic>
{
    typedef internal::ContainerBase<Allocator, Allocator::IsStatic> Base;

protected:
    Container();
    Container(const Allocator& allocator);

    template<typename T>
    T* construct();
    template<typename T, typename ARG1>
    T* construct(ARG1 arg1);
    template<typename T, typename ARG1, typename ARG2>
    T* construct(ARG1 arg1, ARG2 arg2);
    template<typename T, typename ARG1, typename ARG2, typename ARG3>
    T* construct(ARG1 arg1, ARG2 arg2, ARG3 arg3);
    template<typename T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
    T* construct(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4);
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
