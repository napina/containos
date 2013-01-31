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
#ifndef containos_singleton_inl
#define containos_singleton_inl

namespace containos {

template<typename T,typename Allocator>
T* Singleton<T,Allocator>::s_instance = nullptr;

template<typename T,typename Allocator>
__forceinline void Singleton<T,Allocator>::createInstance()
{
    containos_assert(!hasInstance());
    s_instance = containos_placement_new(Allocator::alloc(sizeof(T),4),T);
}

template<typename T,typename Allocator>
__forceinline void Singleton<T,Allocator>::deleteInstance()
{
    containos_assert(hasInstance());
    containos_placement_delete(s_instance,T);
    Allocator::dealloc(s_instance);
    s_instance = nullptr;
}

template<typename T,typename Allocator>
__forceinline bool Singleton<T,Allocator>::hasInstance()
{
    return s_instance != nullptr;
}

template<typename T,typename Allocator>
__forceinline T& Singleton<T,Allocator>::getInstance()
{
    containos_assert(hasInstance());
    return *s_instance;
}

} // end of containos

#endif
