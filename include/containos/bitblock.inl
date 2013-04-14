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
#ifndef containos_bitblock_inl
#define containos_bitblock_inl

namespace containos {

template<typename T>
struct BitBlock<T>::iterator {
    iterator()                                      : m_block(nullptr), m_bits() {}
    iterator(BitBlock<T>* block)                    : m_block(block), m_bits(block->m_mask) {}
    void operator++()                               { m_bits.pop(); }
    T& operator*()                                  { return (*m_block)[m_bits.highest()]; }
    T& operator->()                                 { return (*m_block)[m_bits.highest()]; }
    bool operator==(iterator const& other) const    { return m_bits == other.m_bits; }
    bool operator!=(iterator const& other) const    { return m_bits != other.m_bits; }

private:
    BitBlock<T>* m_block;
    bitset m_bits;
};

template<typename T>
struct BitBlock<T>::const_iterator {
    const_iterator()                                    : m_block(nullptr), m_bits() {}
    const_iterator(BitBlock<T> const* block)            : m_block(block), m_bits(block->m_mask) {}
    void operator++()                                   { m_bits.pop(); }
    T const& operator*()                                { return (*m_block)[m_bits.highest()]; }
    T const& operator->()                               { return (*m_block)[m_bits.highest()]; }
    bool operator==(const_iterator const& other) const  { return m_bits == other.m_bits; }
    bool operator!=(const_iterator const& other) const  { return m_bits != other.m_bits; }

private:
    BitBlock<T>* m_block;
    bitset m_bits;
};
//----------------------------------------------------------------------------

template<typename T>
inline BitBlock<T>::~BitBlock()
{
    clear();
}

template<typename T>
inline BitBlock<T>::BitBlock()
{
}

template<typename T>
inline BitBlock<T>::BitBlock(const BitBlock& other)
    : m_mask(other.mask)
{
    // TODO
}

template<typename T>
inline T& BitBlock<T>::acquire(size_t& index)
{
    index = m_mask.acquire();
    return *containos_placement_new(&m_data[index*sizeof(T)], T);
}

template<typename T>
inline size_t BitBlock<T>::insert(T& item)
{
    size_t index = m_mask.acquire();
    containos_placement_copy(&m_data[index*sizeof(T)], T, item);
    return index;
}

template<typename T>
inline size_t BitBlock<T>::insert(T const& item)
{
    size_t index = m_mask.acquire();
    containos_placement_copy(&m_data[index*sizeof(T)], T, item);
    return index;
}

template<typename T>
inline void BitBlock<T>::remove(size_t index)
{
    containos_assert(m_mask.isSet(index));
    containos_placement_delete(&m_data[index*sizeof(T)], T);
    m_mask.remove(index);
}

template<typename T>
inline void BitBlock<T>::clear()
{
    while(m_mask != 0) {
        size_t index = m_mask.highest();
        containos_placement_delete(&m_data[index*sizeof(T)],T);
        m_mask.remove(index);
    }
}

template<typename T>
inline typename BitBlock<T>::iterator BitBlock<T>::begin()
{
    return iterator(this);
}

template<typename T>
inline typename BitBlock<T>::iterator BitBlock<T>::end()
{
    return iterator();
}

template<typename T>
inline typename BitBlock<T>::const_iterator BitBlock<T>::begin() const
{
    return iterator(this);
}

template<typename T>
inline typename BitBlock<T>::const_iterator BitBlock<T>::end() const
{
    return iterator();
}

template<typename T>
T& BitBlock<T>::operator[](size_t index)
{
    containos_assert(m_mask.isSet(index));
    return *reinterpret_cast<T*>(&m_data[index*sizeof(T)]);
}

template<typename T>
T const& BitBlock<T>::operator[](size_t index) const
{
    containos_assert(m_mask.isSet(index));
    return *reinterpret_cast<T*>(&m_data[index*sizeof(T)]);
}

template<typename T>
__forceinline size_t BitBlock<T>::size() const
{
    return m_mask.count();
}

template<typename T>
__forceinline size_t BitBlock<T>::capasity() const
{
    return bitset::num_bits;
}

} // end of containos

#endif
