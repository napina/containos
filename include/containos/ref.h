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

#include "containos\config.h"

namespace containos {

// add this to class header
#define REF_STORAGE(ClassType,SizeType)\
    friend class containos::Ref<ClassType>;\
    mutable SizeType m_refCount
// add this to constructor
#define REF_STORAGE_INIT()\
    m_refCount(0)
#define REF_STORAGE_RESET()\
    m_refCount = 0

// reference counted scoped pointer
template<typename T>
class Ref
{
public:
    ~Ref();
    Ref();
    Ref(nullptr_t);
    Ref(T const* __restrict ptr);
    Ref(Ref<T> const& handle);

    Ref<T>& operator=(T const* ptr);
    Ref<T>& operator=(Ref<T> const& handle);
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

    void reset(T const* ptr = nullptr);
    /// doesn't delete
    T* release();

private:
    void set(T const* ptr);
    void removeRef();

private:
    T* m_mem;
};

} // end of containos

#include "containos/ref.inl"

#endif
