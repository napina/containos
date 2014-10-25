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
#ifndef containos_stackallocator_h
#define containos_stackallocator_h

namespace containos {

//--------------------------------------------------------------------------*/
#define stack_allocator(size)\
    void* stackMem = alloca(size);\
    containos::StackAllocator stackAllocator(stackMem, size);

//--------------------------------------------------------------------------*/
struct StackAllocator
{
    StackAllocator(void* ptr, size_t capacity);

    void* alloc(size_t size, size_t alignment);
    void  dealloc(void* ptr);

private:
    StackAllocator(StackAllocator const& other);
    void operator=(StackAllocator const& other);

    void* m_ptr;
    size_t m_size;
    size_t m_capacity;
};

} // end of containos

#include "containos/stackallocator.inl"

#endif
