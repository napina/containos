/*=============================================================================

Copyright (c) 2007-2013 Ville Ruusutie

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
#ifndef containos_ptr_h
#define containos_ptr_h

namespace containos {

template<typename T>
class Ptr
{
public:
    ~Ptr();
    Ptr();
    explicit Ptr(T* ptr);
    Ptr(Ptr<T>& other);

    Ptr<T>& operator=(T* ptr);
    Ptr<T>& operator=(Ptr<T>& handle);
    Ptr<T>& operator=(nullptr_t);
    bool operator==(T* other) const;
    bool operator!=(T* other) const;
    bool operator<(const Ptr<T>& other) const;

    operator bool() const;
    operator T*() const;
    operator T const*() const;
    T const& operator*() const;
    T& operator*();
    T const* operator->() const;
    T* operator->();
    T const* get() const;
    T* get();

    template<typename T2> T2* cast();

    void reset(T* ptr = nullptr);
    /// does not delete! this removes ownership and returns pointer
    T* release();

private:
    T* m_ptr;
};

} // end of containos

#include "containos/ptr.inl"

#endif
