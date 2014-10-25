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
#ifndef containos_stackallocator_inl
#define containos_stackallocator_inl

namespace containos {

__forceinline StackAllocator::StackAllocator(void* ptr, size_t size)
    : m_ptr(ptr)
    , m_size(size)
    , m_capacity(0)
{
}

__forceinline StackAllocator::StackAllocator(StackAllocator const& other)
    : m_ptr(other.m_ptr)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
{
    // this is evil
}

__forceinline void* StackAllocator::alloc(size_t size, size_t alignment)
{
    alignment; // TODO
    void* allocPtr = reinterpret_cast<char*>(m_ptr) + m_size;
    m_size += size;
    return allocPtr;
}

__forceinline void StackAllocator::dealloc(void*)
{
    // do nothing
}

} // end of containos

#endif
