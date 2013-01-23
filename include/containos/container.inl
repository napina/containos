// Copyright (C) Ville Ruusutie, 2008

#pragma once
#ifndef korppu_container_inl
#define korppu_container_inl

#include "korppu/memory/memory.h"

namespace k {

template<typename AllocatorType>
__forceinline Container<AllocatorType>::Container()
    : Base()
{
}

template<typename AllocatorType>
__forceinline Container<AllocatorType>::Container(const AllocatorType& allocator)
    : Base(allocator)
{
}

template<typename AllocatorType>
template<typename T>
__forceinline T* Container<AllocatorType>::construct()
{
    return k_placement_new(Base::allocate(sizeof(T)), T);
}

template<typename AllocatorType>
template<typename T, typename ARG1>
__forceinline T* Container<AllocatorType>::construct(ARG1 arg1)
{
    return k_placement_new1(Base::allocate(sizeof(T)), T, arg1);
}

template<typename AllocatorType>
template<typename T, typename ARG1, typename ARG2>
__forceinline T* Container<AllocatorType>::construct(ARG1 arg1, ARG2 arg2)
{
    return k_placement_new2(Base::allocate(sizeof(T)), T, arg1, arg2);
}

template<typename AllocatorType>
template<typename T, typename ARG1, typename ARG2, typename ARG3>
__forceinline T* Container<AllocatorType>::construct(ARG1 arg1, ARG2 arg2, ARG3 arg3)
{
    return k_placement_new3(Base::allocate(sizeof(T)), T, arg1, arg2, arg3);
}

template<typename AllocatorType>
template<typename T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
__forceinline T* Container<AllocatorType>::construct(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4)
{
    return k_placement_new4(Base::allocate(sizeof(T)), T, arg1, arg2, arg3, arg4);
}

template<typename AllocatorType>
template<typename T>
__forceinline void Container<AllocatorType>::destruct(T* ptr)
{
    k_placement_delete(ptr, T);
    Base::deallocate(ptr);
}

template<typename AllocatorType>
template<typename T>
__forceinline T* Container<AllocatorType>::constructArray(size_t size)
{
    void* buffer = Base::allocate(sizeof(T) * size);
    return reinterpret_cast<T*>(buffer);
}

template<typename AllocatorType>
template<typename T>
__forceinline void Container<AllocatorType>::destructArray(T* ptr, size_t)
{
    Base::deallocate(ptr);
}

} // end of korppu

#endif
