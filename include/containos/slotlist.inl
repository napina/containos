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
#ifndef containos_slotlist_inl
#define containos_slotlist_inl

namespace containos {

template<typename T,typename Allocator>
inline SlotList<T,Allocator>::~SlotList()
{
    clear();
}

template<typename T,typename Allocator>
inline SlotList<T,Allocator>::SlotList()
    : m_chunkList(100)
    , m_freeList(100)
{
}

template<typename T,typename Allocator>
inline SlotList<T,Allocator>::SlotList(SlotList const& other)
{
    // TODO
}

template<typename T,typename Allocator>
__forceinline typename SlotList<T,Allocator>::id_t SlotList<T,Allocator>::create()
{
    if(m_freeList.empty()) {
        T* chunk = Base::constructArray(chunk_size);
        for(int i = chunk_size - 1; i >= 0; --i) {
            //chunk[i].id = object_table.size() * chunk_size + i;
            m_freeList.push_back(m_chunkList.size() * chunk_size + i);
        }
        m_chunkList.insert(chunk);
    }

    int free = m_freeList.back();
    m_freeList.pop_back();
    return m_chunkList[free / chunk_size][free % chunk_size].id;
}

template<typename T,typename Allocator>
inline void SlotList<T,Allocator>::remove(id_t id)
{
    T* obj = operator[](id);
    obj->id = (obj->id & 0xFFFFFFFF) | (((obj->id >> 32) + 1) << 32);
    m_freeList.push_back(id & 0xFFFFFFFF);
}

template<typename T,typename Allocator>
__forceinline void SlotList<T,Allocator>::clear()
{
//    m_size = 0;
}

template<typename T,typename Allocator>
__forceinline T& SlotList<T,Allocator>::operator[](id_t id)
{
    T* obj = m_chunkList[(id & 0xFFFFFFFF) / chunk_size] + ((id & 0xFFFFFFFF) % chunk_size);
    return &obj;// TODO obj->id != id ? nullptr : obj;
}

template<typename T,typename Allocator>
__forceinline T const& SlotList<T,Allocator>::operator[](id_t id) const
{
    T* obj = m_chunkList[(id & 0xFFFFFFFF) / chunk_size] + ((id & 0xFFFFFFFF) % chunk_size);
    return &obj;// TODO obj->id != id ? nullptr : obj;
}

template<typename T,typename Allocator>
__forceinline int SlotList<T,Allocator>::size() const
{
    return 0;//m_size;
}

template<typename T,typename Allocator>
__forceinline bool SlotList<T,Allocator>::isEmpty() const
{
    return false;//m_size == 0;
}

} // end of containos

#endif
