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
#ifndef containos_slotlist_h
#define containos_slotlist_h

#include "containos\container.h"
#include "containos\list.h" // TODO remove dependency
#include "containos\list.h" // TODO remove dependency

namespace containos {

// Auto grow list where items keep same location
template<typename T,typename Allocator=Mallocator>
class SlotList : protected Container<Allocator>
{
    typedef Container<Allocator> Base;
public:
    ~SlotList();
    SlotList();
    SlotList(SlotList const& other);

    uint64_t acquire();
    void remove(uint64_t id);
    void clearAndFree();
    void clear();

    T* operator[](uint64_t id);
    T const* operator[](uint64_t id) const;
    int size() const;
    bool isEmpty() const;

private:
    struct Chunk;
    static const int chunk_size = 128;
    List<Chunk*,Allocator> m_chunkList;
    List<int,Allocator> m_freeList;
};

} // end of containos

#include "containos/slotlist.inl"

#endif
