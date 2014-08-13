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
#ifndef containos_ref_h
#define containos_ref_h

#include "containos\common.h"

namespace containos {

// Reference counted smart pointer. Add storage macros to classes or use override by defining
// void addReference() and uint32_t removeReference()

// add this to class header. this needs to be protected member to support derived access
#define REF_STORAGE(ClassType,SizeType)             REF_STORAGE_IMPL(ClassType,SizeType)
// add this to class header. this needs to be protected member to support derived access
#define REF_STORAGE_BITS(ClassType,SizeType,Bits)   REF_STORAGE_BITS_IMPL(ClassType,SizeType,Bits)
// add this to derived class
#define REF_DERIVED(ClassType)                      REF_DERIVED_IMPL(ClassType)
// add this to constructor
#define REF_STORAGE_INIT()                          REF_STORAGE_INIT_IMPL()
// use this if you want to reset count
#define REF_STORAGE_RESET()                         REF_STORAGE_RESET_IMPL()

// reference counted scoped pointer
template<typename T>
class Ref
{
public:
    ~Ref();
    Ref();
    Ref(nullptr_t);
    Ref(T* __restrict ptr);
    explicit Ref(Ref<T>& other);

    Ref<T>& operator=(T* ptr);
    Ref<T>& operator=(Ref<T>& other);
    Ref<T>& operator=(nullptr_t);
    bool operator==(Ref<T> const& other) const;
    bool operator!=(Ref<T> const& other) const;
    bool operator==(T const* other) const;
    bool operator!=(T const* other) const;
    bool operator<(Ref<T> const& other) const;

    operator bool() const;
    operator T*() const;
    operator T const*() const;
    T& operator*();
    T* operator->();
    T const& operator*() const;
    T const* operator->() const;
    T const* get() const;
    T* get();
    bool isValid() const;

    template<typename T2> T2* cast();

    void reset(T* ptr = nullptr);
    /// doesn't delete
    T* release();

private:
    void set(T* ptr);
    void removeRef();

private:
    T* m_mem;
};

} // end of containos

#include "containos/ref.inl"

#endif
