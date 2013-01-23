// Copyright (C) Ville Ruusutie, 2008

#pragma once
#ifndef korppu_containerbase_h
#define korppu_containerbase_h

namespace k {
namespace internal {

template<typename AllocatorType, bool IsStatic>
class ContainerBase;

template<typename AllocatorType>
class ContainerBase<AllocatorType, true>
{
protected:
    void* allocate(size_t size, int flags = 0);
    void* allocateAligned(size_t size, size_t alignment, int flags = 0);
    void* reallocate(void* ptr, size_t size);
    void  deallocate(void* ptr);
    void  deallocateAligned(void* ptr);
};

template<typename AllocatorType>
class ContainerBase<AllocatorType, false>
{
protected:
    ContainerBase();
    ContainerBase(const AllocatorType& allocator);

    void* allocate(size_t size, int flags = 0);
    void* allocateAligned(size_t size, size_t alignment, int flags = 0);
    void* reallocate(void* ptr, size_t size);
    void  deallocate(void* ptr);
    void  deallocateAligned(void* ptr);

private:
    AllocatorType m_allocator;
};

} // end of internal
} // end of korppu

#include "korppu/container/containerbase.inl"

#endif
