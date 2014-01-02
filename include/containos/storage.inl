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
#ifndef containos_storage_inl
#define containos_storage_inl

namespace containos {

template<typename T,typename Allocator>
__forceinline LinearStorage* LinearStorage::alloc(size_t capasity)
{
	return static_cast<T*>(Allocator::alloc(capasity + sizeof(T)));
}

template<typename T,typename Allocator>
__forceinline LinearStorage* LinearStorage::alloc(size_t capasity, Allocator& allocator)
{
	return static_cast<T*>(allocator.alloc(capasity + sizeof(T)));
}

template<typename Allocator>
__forceinline void LinearStorage::free(LinearStorage* ptr)
{
	Allocator::free(ptr);
}

template<typename Allocator>
__forceinline void LinearStorage::free(LinearStorage* ptr, Allocator& allocator)
{
	allocator.free(ptr);
}

template<typename T>
__forceinline T* LinearStorage::get(size_t index)
{
	return static_cast<T*>(m_mem + sizeof(T) * index);
}

__forceinline void* LinearStorage::get(size_t offset)
{
	return (char*)m_mem + offset;
}

__forceinline void LinearStorage::copyTo(LinearStorage& target)
{
    target;
	containos_todo("Implement copyTo");
}

__forceinline void LinearStorage::copyTo(ChunkStorage& target)
{
    target;
	containos_todo("Implement copyTo");
}

} // end of containos

#endif
