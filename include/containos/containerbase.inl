// Copyright (C) Ville Ruusutie, 2008

#pragma once
#ifndef korppu_containerbase_inl
#define korppu_containerbase_inl

namespace k {
namespace internal {

template<typename AllocatorType>
__forceinline void* ContainerBase<AllocatorType,true>::allocate(size_t size, int flags)
{
    return AllocatorType::allocate(size, flags);
}

template<typename AllocatorType>
__forceinline void* ContainerBase<AllocatorType,true>::allocateAligned(size_t size, size_t alignment, int flags)
{
    return AllocatorType::allocateAligned(size, alignment, flags);
}

template<typename AllocatorType>
__forceinline void* ContainerBase<AllocatorType,true>::reallocate(void* pPtr, size_t size)
{
    return AllocatorType::reallocate(pPtr, size);
}

template<typename AllocatorType>
__forceinline void ContainerBase<AllocatorType,true>::deallocate(void* pPtr)
{
    AllocatorType::deallocate(pPtr);
}

template<typename AllocatorType>
__forceinline void ContainerBase<AllocatorType,true>::deallocateAligned(void* pPtr)
{
    AllocatorType::deallocateAligned(pPtr);
}

template<typename AllocatorType>
__forceinline ContainerBase<AllocatorType,false>::ContainerBase()
{
}

template<typename AllocatorType>
__forceinline ContainerBase<AllocatorType,false>::ContainerBase(const AllocatorType& allocator)
    : m_allocator(allocator)
{
}

template<typename AllocatorType>
__forceinline void* ContainerBase<AllocatorType,false>::allocate(size_t size, int flags)
{
    return m_allocator.allocate(size, flags);
}

template<typename AllocatorType>
__forceinline void* ContainerBase<AllocatorType,false>::allocateAligned(size_t size, size_t alignment, int flags)
{
    return m_allocator.allocateAligned(size, alignment, flags);
}

template<typename AllocatorType>
__forceinline void* ContainerBase<AllocatorType,false>::reallocate(void* pPtr, size_t size)
{
    return m_allocator.reallocate(pPtr, size);
}

template<typename AllocatorType>
__forceinline void ContainerBase<AllocatorType,false>::deallocate(void* pPtr)
{
    m_allocator.deallocate(pPtr);
}

template<typename AllocatorType>
__forceinline void ContainerBase<AllocatorType,false>::deallocateAligned(void* pPtr)
{
    m_allocator.deallocateAligned(pPtr);
}

} // end of internal
} // end of korppu

#endif
