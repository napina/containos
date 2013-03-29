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
#ifndef containos_hashmap_h
#define containos_hashmap_h

#include "containos/container.h"

namespace containos {

/*
    Key must have Hash() and compare operator methods!

    Examples:
        HashMap<int,Data> numberToData;
        HashMap<void*,Data> pointerToData;
        HashMap<HashedString,Data,DataPool> stringToData;
*/

typedef unsigned int uint32;

namespace internal {

template<typename Key> struct HashedKeyToBucket;
template<typename Key> struct PointerKeyToBucket;
template<typename Key> struct IntegerKeyToBucket;
template<typename Key,typename T> class HashMapNode;

template<typename Key,typename T,typename ToBucket,typename Allocator>
class HashMapBase : protected Container<Allocator>
{
    typedef Container<Allocator> Base;
    typedef Allocator AllocatorType;
    typedef HashMapNode<Key,T> NodeType;
    friend class const_iterator;
    friend class iterator;
public:
    typedef T ElementType;
    typedef Key KeyType;

    struct iterator
    {
        typedef HashMapBase<Key,T,ToBucket,Allocator> MapType;
        typedef HashMapNode<Key,T> NodeType;

        iterator(MapType* map);
        iterator(MapType* map, NodeType* node, uint32 bucketIndex);
        iterator(iterator const& other);

        void next();
        NodeType* node() const;
        T& operator*() const;
        T* operator->() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(iterator const& other) const;
        bool operator!=(iterator const& other) const;

    private:
        MapType* m_map;
        NodeType* m_node;
        uint32 m_bucketIndex;
    };

    struct const_iterator
    {
    public:
        typedef HashMapBase<Key,T,ToBucket,Allocator> MapType;
        typedef HashMapNode<Key,T> NodeType;

        const_iterator(MapType* map);
        const_iterator(MapType* map, NodeType* node, uint32 bucketIndex);
        const_iterator(iterator const& other);
        const_iterator(const_iterator const& other);

        void next();
        NodeType const* node() const;
        T const& operator*() const;
        T const* operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const_iterator const& other) const;
        bool operator!=(const_iterator const& other) const;

    private:
        MapType const* m_map;
        NodeType const* m_node;
        uint32 m_bucketIndex;
    };

    ~HashMapBase();
    HashMapBase(uint32 bucketCount);

    bool insert(Key const& key, T& value);
    bool insert(Key const& key, T const& value);
    void remove(Key const& key);
    void clear();

    uint32 size() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    iterator find(Key const& key);

private:
    NodeType* createHashNode(Key const& key, T& value, NodeType* next);
    NodeType* createHashNode(Key const& key, T const& value, NodeType* next);

private:
    NodeType** m_buckets;
    uint32 m_bucketCount;
    uint32 m_size;
};

} // end of internal

template<typename Key,typename T,typename Allocator=Mallocator>
struct HashMap :
    public internal::HashMapBase<Key,T,internal::HashedKeyToBucket<Key>,Allocator>
{
    ~HashMap();
    HashMap(uint32 bucketCount);
};

template<typename Key,typename T,typename Allocator>
struct HashMap<Key*,T,Allocator> :
    public internal::HashMapBase<Key*,T,internal::PointerKeyToBucket<Key*>,Allocator>
{
    ~HashMap();
    HashMap(uint32 bucketCount);
};

template<typename T,typename Allocator>
struct HashMap<uint32,T,Allocator> :
    public internal::HashMapBase<uint32,T,internal::IntegerKeyToBucket<uint32>,Allocator>
{
    ~HashMap();
    HashMap(uint32 bucketCount);
};

} // end of korppu

#include "containos/hashmap.inl"

#endif
