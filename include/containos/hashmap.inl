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
#ifndef containos_hashmap_inl
#define containos_hashmap_inl

namespace containos {
namespace internal {

template<typename Key>
struct HashedKeyToBucket
{
    static uint32 get(Key const& key, uint32 bucketCount);
};

template<typename Key>
struct PointerKeyToBucket
{
    static uint32 get(Key const& key, uint32 bucketCount);
};

template<typename Key>
struct IntegerKeyToBucket
{
    static uint32 get(Key const& key, uint32 bucketCount);
};

template<typename Key,typename T>
class HashMapNode
{
public:
    ~HashMapNode();
    HashMapNode(Key const& key, T& value, HashMapNode<Key,T>* next);
    HashMapNode(Key const& key, T const& value, HashMapNode<Key,T>* next);

    HashMapNode* m_next;
    Key m_key;
    T m_value;
};

template<typename Key>
__forceinline uint32 HashedKeyToBucket<Key>::get(Key const& key, uint32 bucketCount)
{
    return key.Hash() % bucketCount;
}

template<typename Key>
__forceinline uint32 PointerKeyToBucket<Key>::get(Key const& key, uint32 bucketCount)
{
    // todo shift down by alignment
    return reinterpret_cast<uint32>(key) % bucketCount;
}

template<typename Key>
__forceinline uint32 IntegerKeyToBucket<Key>::get(Key const& key, uint32 bucketCount)
{
    return static_cast<uint32>(key) % bucketCount;
}

template<typename Key,typename T>
__forceinline HashMapNode<Key,T>::~HashMapNode()
{
}

template<typename Key,typename T>
__forceinline HashMapNode<Key,T>::HashMapNode(Key const& key, T& value, HashMapNode<Key,T>* next)
    : m_next(next)
    , m_key(key)
    , m_value(value)
{
}

template<typename Key,typename T>
__forceinline HashMapNode<Key,T>::HashMapNode(Key const& key, T const& value, HashMapNode<Key,T>* next)
    : m_next(next)
    , m_key(key)
    , m_value(value)
{
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::iterator::iterator(HashMapBase<Key,T,ToBucket,Allocator>* map)
    : m_map(map)
    , m_node(0)
    , m_bucketIndex(0)
{
    while(m_bucketIndex < m_map->bucketCount) {
        m_node = m_map->buckets[m_bucketIndex];
        if(m_node)
            break;
        ++(m_bucketIndex);
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::iterator::iterator(HashMapBase<Key,T,ToBucket,Allocator>* map, HashMapNode<Key,T>* node, uint32 bucketIndex)
    : m_map(map)
    , m_node(node)
    , m_bucketIndex(bucketIndex)
{
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::iterator::iterator(iterator const& other)
    : m_map(other.m_map)
    , m_node(other.m_node)
    , m_bucketIndex(other.m_bucketIndex)
{
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline void HashMapBase<Key,T,ToBucket,Allocator>::iterator::next()
{
    m_node = m_node->next;
    if(m_node == 0) {
        ++(m_bucketIndex);
        while(m_bucketIndex < m_map->bucketCount) {
            m_node = m_map->buckets[m_bucketIndex];
            if(m_node)
                break;
            ++(m_bucketIndex);
        }
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapNode<Key,T>* HashMapBase<Key,T,ToBucket,Allocator>::iterator::node() const
{
    return m_node;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline T& HashMapBase<Key,T,ToBucket,Allocator>::iterator::operator*() const
{
    return m_node->value;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline T* HashMapBase<Key,T,ToBucket,Allocator>::iterator::operator->() const
{
    return &(m_node->value);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::iterator& HashMapBase<Key,T,ToBucket,Allocator>::iterator::operator++()
{
    Next();
    return *this;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::iterator HashMapBase<Key,T,ToBucket,Allocator>::iterator::operator++(int)
{
    iterator temp(*this);
    Next();
    return temp;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline bool HashMapBase<Key,T,ToBucket,Allocator>::iterator::operator==(iterator const& other) const
{
    return m_node == other.node;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline bool HashMapBase<Key,T,ToBucket,Allocator>::iterator::operator!=(iterator const& other) const
{
    return m_node != other.node;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::const_iterator(HashMapBase<Key,T,ToBucket,Allocator>* map)
    : map(map)
    , node(0)
    , bucketIndex(0)
{
    while(m_bucketIndex < m_map->bucketCount) {
        m_node = m_map->buckets[index];
        if(m_node)
            break;
        ++(m_bucketIndex);
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::const_iterator(HashMapBase<Key,T,ToBucket,Allocator>* map, HashMapNode<Key,T>* node, uint32 bucketIndex)
    : map(map)
    , node(node)
    , bucketIndex(bucketIndex)
{
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::const_iterator(
        typename HashMapBase<Key,T,ToBucket,Allocator>::iterator const& other)
    : map(other.map)
    , node(other.node)
    , bucketIndex(other.bucketIndex)
{
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::const_iterator(const_iterator const& other)
    : map(other.map)
    , node(other.node)
    , bucketIndex(other.bucketIndex)
{
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline void HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::next()
{
    m_node = m_node->next;
    if(m_node == 0) {
        ++(m_bucketIndex);
        while(m_bucketIndex < m_map->bucketCount) {
            m_node = m_map->buckets[m_bucketIndex];
            if(m_node)
                break;
            ++(m_bucketIndex);
        }
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapNode<Key,T> const* HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::node() const
{
    return m_node;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline T const& HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::operator*() const
{
    return m_node->value;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline T const* HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::operator->() const
{
    return &(m_node->value);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::const_iterator& HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::operator++()
{
    next();
    return *this;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::const_iterator HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::operator++(int)
{
    const_iterator temp(*this);
    next();
    return temp;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline bool HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::operator==(const_iterator const& other) const
{
    return m_node == other.node;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline bool HashMapBase<Key,T,ToBucket,Allocator>::const_iterator::operator!=(const_iterator const& other) const
{
    return m_node != other.node;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
inline HashMapBase<Key,T,ToBucket,Allocator>::~HashMapBase()
{
    clear();
    Base::template destructArray(m_buckets, m_bucketCount);
    m_buckets = 0;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapBase<Key,T,ToBucket,Allocator>::HashMapBase(uint32 bucketCount)
    : Base()
    , m_bucketCount(bucketCount)
    , m_size(0)
{
    m_buckets = Base::template constructArray<HashMapNode<Key,T>*>(bucketCount);
    ::memset(m_buckets, 0, sizeof(HashMapNode<Key,T>*) * bucketCount);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
inline bool HashMapBase<Key,T,ToBucket,Allocator>::insert(Key const& key, T& value)
{
    T* oldValue = find(key);
    if(oldValue != 0) {
        *oldValue = value;
        return false;
    } else {
        // insert as first in the linked list
        uint32 bucketIndex = ToBucket::get(key, m_bucketCount);
        m_buckets[bucketIndex] = CreateHashNode(key, value, m_buckets[bucketIndex]);
        ++(m_size);
        return true;
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
inline bool HashMapBase<Key,T,ToBucket,Allocator>::insert(Key const& key, T const& value)
{
    T* oldValue = find(key);
    if(oldValue != 0) {
        *oldValue = value;
        return false;
    } else {
        // insert as first in the linked list
        uint32 bucketIndex = ToBucket::get(key, m_bucketCount);
        m_buckets[bucketIndex] = createHashNode(key, value, m_buckets[bucketIndex]);
        ++(m_size);
        return true;
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
inline void HashMapBase<Key,T,ToBucket,Allocator>::remove(Key const& key)
{
    uint32 bucketIndex = ToBucket::get(key, m_bucketCount);
    HashMapNode<Key,T>* node = m_buckets[bucketIndex];
    HashMapNode<Key,T>* prevNode = 0;
    while(node) {
        if(node->key == key) {
            if(prevNode) {
                prevNode->next = node->next;
            } else {
                m_buckets[bucketIndex] = node->next;
            }
            Base::template destruct<HashMapNode<Key,T> >(node);
            --(m_size);
            return;
        }
        prevNode = node;
        node = node->next;
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
inline T* HashMapBase<Key,T,ToBucket,Allocator>::find(Key const& key) const
{
    uint32 bucketIndex = ToBucket::get(key, m_bucketCount);
    for(HashMapNode<Key,T>* node = m_buckets[bucketIndex]; node; node = node->m_next) {
        if(node->m_key == key) {
            return &(node->m_value);
        }
    }
    return 0;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
inline void HashMapBase<Key,T,ToBucket,Allocator>::clear()
{
    for(uint32 i = 0; i < m_bucketCount; ++i) {
        // delete bucket and linked list it owns
        HashMapNode<Key,T>* bucketNode = m_buckets[i];
        while(bucketNode) {
            HashMapNode<Key,T>* nextBucketNode = bucketNode->m_next;
            Base::template destruct<HashMapNode<Key,T> >(bucketNode);
            bucketNode = nextBucketNode;
        }
        m_buckets[i] = 0;
    }
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline uint32 HashMapBase<Key,T,ToBucket,Allocator>::size() const
{
    return m_size;
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::iterator HashMapBase<Key,T,ToBucket,Allocator>::begin()
{
    return iterator(this);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::iterator HashMapBase<Key,T,ToBucket,Allocator>::end()
{
    return iterator(this, 0, 0);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::const_iterator HashMapBase<Key,T,ToBucket,Allocator>::begin() const
{
    return const_iterator(this);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline typename HashMapBase<Key,T,ToBucket,Allocator>::const_iterator HashMapBase<Key,T,ToBucket,Allocator>::end() const
{
    return const_iterator(this, 0, 0);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapNode<Key,T>* HashMapBase<Key,T,ToBucket,Allocator>::createHashNode(Key const& key, T& value, HashMapNode<Key,T>* next)
{
    return Base::template construct<
        HashMapNode<Key,T>, Key const&, T&, HashMapNode<Key,T>*>(
            key, value, next);
}

template<typename Key,typename T,typename ToBucket,typename Allocator>
__forceinline HashMapNode<Key,T>* HashMapBase<Key,T,ToBucket,Allocator>::createHashNode(Key const& key, T const& value, HashMapNode<Key,T>* next)
{
    return Base::template construct<
        HashMapNode<Key,T>, Key const&, T const&, HashMapNode<Key,T>*>(
            key, value, next);
}

} // end of internal

template<typename Key,typename T,typename Allocator>
__forceinline HashMap<Key,T,Allocator>::~HashMap()
{
    //HashMapBase::~HashMapBase();
}

template<typename Key,typename T,typename Allocator>
__forceinline HashMap<Key,T,Allocator>::HashMap(uint32 bucketCount)
    : internal::HashMapBase<Key,T,internal::HashedKeyToBucket<Key>,Allocator>(bucketCount)
{
}

template<typename Key,typename T,typename Allocator>
__forceinline HashMap<Key*,T,Allocator>::~HashMap()
{
    //HashMapBase::~HashMapBase();
}

template<typename Key,typename T,typename Allocator>
__forceinline HashMap<Key*,T,Allocator>::HashMap(uint32 bucketCount)
    : internal::HashMapBase<Key*,T,internal::PointerKeyToBucket<Key*>,Allocator>(bucketCount)
{
}

template<typename T,typename Allocator>
__forceinline HashMap<uint32,T,Allocator>::~HashMap()
{
    //HashMapBase::~HashMapBase();
}

template<typename T,typename Allocator>
__forceinline HashMap<uint32,T,Allocator>::HashMap(uint32 bucketCount)
    : internal::HashMapBase<uint32,T,internal::IntegerKeyToBucket<uint32>,Allocator>(bucketCount)
{
}

} // end of containos

#endif
