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
#ifndef containos_container_inl
#define containos_container_inl

namespace containos {

//----------------------------------------------------------------------------
namespace internal {

template<typename Allocator, bool IsStatic>
struct ContainerBase
{
protected:
    void* alloc(size_t size, size_t align, int flags = 0)   { return Allocator::alloc(size, align, flags); }
    void  dealloc(void* ptr)                                { Allocator::dealloc(ptr); }
};

template<typename Allocator>
struct ContainerBase<Allocator,false>
{
    void setAllocator(Allocator* allocator)                 { m_allocator = allocator; }

protected:
    void* alloc(size_t size, size_t align, int flags = 0)   { return m_allocator->alloc(size, align, flags); }
    void  dealloc(void* ptr)                                { m_allocator->dealloc(ptr); }

private:
    Allocator* m_allocator;
};

} // end of internal
//----------------------------------------------------------------------------

template<typename Allocator>
template<typename T>
__forceinline T* Container<Allocator>::construct()
{
    return containos_placement_new(Base::alloc(sizeof(T),__alignof(T)), T);
}

template<typename Allocator>
template<typename T, typename A>
__forceinline T* Container<Allocator>::construct(A a)
{
    return containos_placement_new1(Base::alloc(sizeof(T),__alignof(T)), T, a);
}

template<typename Allocator>
template<typename T, typename A, typename B>
__forceinline T* Container<Allocator>::construct(A a, B b)
{
    return containos_placement_new2(Base::alloc(sizeof(T),__alignof(T)), T, a, b);
}

template<typename Allocator>
template<typename T, typename A, typename B, typename C>
__forceinline T* Container<Allocator>::construct(A a, B b, C c)
{
    return containos_placement_new3(Base::alloc(sizeof(T),__alignof(T)), T, a, b, c);
}

template<typename Allocator>
template<typename T, typename A, typename B, typename C, typename D>
__forceinline T* Container<Allocator>::construct(A a, B b, C c, D d)
{
    return containos_placement_new4(Base::alloc(sizeof(T),__alignof(T)), T, a, b, c, d);
}

template<typename Allocator>
template<typename T,typename A,typename B,typename C,typename D,typename E>
__forceinline T* Container<Allocator>::construct(A a, B b, C c, D d, E e)
{
    return containos_placement_new5(Base::alloc(sizeof(T),__alignof(T)), T, a, b, c, d, e);
}

template<typename Allocator>
template<typename T,typename A,typename B,typename C,typename D,typename E,typename F>
__forceinline T* Container<Allocator>::construct(A a, B b, C c, D d, E e, F f)
{
    return containos_placement_new6(Base::alloc(sizeof(T),__alignof(T)), T, a, b, c, d, e, f);
}

template<typename Allocator>
template<typename T,typename A,typename B,typename C,typename D,typename E,typename F,typename G>
__forceinline T* Container<Allocator>::construct(A a, B b, C c, D d, E e, F f, G g)
{
    return containos_placement_new7(Base::alloc(sizeof(T),__alignof(T)), T, a, b, c, d, e, f, g);
}

template<typename Allocator>
template<typename T,typename A,typename B,typename C,typename D,typename E,typename F,typename G,typename H>
__forceinline T* Container<Allocator>::construct(A a, B b, C c, D d, E e, F f, G g, H h)
{
    return containos_placement_new8(Base::alloc(sizeof(T),__alignof(T)), T, a, b, c, d, e, f, g, h);
}

template<typename Allocator>
template<typename T>
__forceinline void Container<Allocator>::destruct(T* ptr)
{
    containos_placement_delete(ptr, T);
    Base::dealloc(ptr);
}

template<typename Allocator>
template<typename T>
__forceinline T* Container<Allocator>::constructArray(size_t size)
{
    void* buffer = Base::alloc(sizeof(T) * size,__alignof(T));
    return reinterpret_cast<T*>(buffer);
}

template<typename Allocator>
template<typename T>
__forceinline void Container<Allocator>::destructArray(T* ptr, size_t)
{
    Base::dealloc(ptr);
}

} // end of containos

#endif
