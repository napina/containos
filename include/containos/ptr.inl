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
#ifndef containos_ptr_inl
#define containos_ptr_inl

namespace containos {

template<typename T>
__forceinline Ptr<T>::~Ptr()
{
    containos_delete(m_ptr);
}

template<typename T>
__forceinline Ptr<T>::Ptr()
    : m_ptr(nullptr)
{
}

template<typename T>
__forceinline Ptr<T>::Ptr(T* ptr)
    : m_ptr(ptr)
{
}

template<typename T>
__forceinline Ptr<T>::Ptr(Ptr<T>& other)
    : m_ptr(other.m_ptr)
{
    other.m_ptr = nullptr;
}

template<typename T>
__forceinline Ptr<T>& Ptr<T>::operator=(T* ptr)
{
    reset(ptr);
    return *this;
}

template<typename T>
__forceinline Ptr<T>& Ptr<T>::operator=(Ptr<T>& other)
{
    reset(other.m_ptr);
    other.m_ptr = nullptr;
    return *this;
}

template<typename T>
__forceinline Ptr<T>& Ptr<T>::operator=(nullptr_t)
{
    containos_delete(m_ptr);
    return *this;
}

template<typename T>
__forceinline bool Ptr<T>::operator==(T* other) const
{
    return (m_ptr == other);
}

template<typename T>
__forceinline bool Ptr<T>::operator!=(T* other) const
{
    return (m_ptr != other);
}

template<typename T>
__forceinline bool Ptr<T>::operator<(const Ptr<T>& other) const
{
    return m_ptr < other.m_ptr;
}

template<typename T>
__forceinline Ptr<T>::operator T*() const
{
    return m_ptr;
}

template<typename T>
__forceinline Ptr<T>::operator const T*() const
{
    return m_ptr;
}

template<typename T>
__forceinline T const& Ptr<T>::operator*() const
{
    containos_assert(m_ptr != nullptr);
    return *m_ptr;
}

template<typename T>
__forceinline T& Ptr<T>::operator*()
{
    containos_assert(m_ptr != nullptr);
    return *m_ptr;
}

template<typename T>
__forceinline T const* Ptr<T>::operator->() const
{
    containos_assert(m_ptr != nullptr);
    return m_ptr;
}

template<typename T>
__forceinline T* Ptr<T>::operator->()
{
    containos_assert(m_ptr != nullptr);
    return m_ptr;
}

template<typename T>
__forceinline T const* Ptr<T>::get() const
{
    return m_ptr;
}

template<typename T>
__forceinline T* Ptr<T>::get()
{
    return m_ptr;
}

template<typename T>
template<typename T2>
__forceinline T2* Ptr<T>::cast()
{
    return containos_cast(T2*, m_ptr);
}

template<typename T>
__forceinline void Ptr<T>::reset(T* ptr)
{
    containos_delete(m_ptr);
    m_ptr = ptr;
}

template<typename T>
__forceinline T* Ptr<T>::release()
{
    T* releasedPtr = m_ptr;
    m_ptr = nullptr;
    return releasedPtr;
}

} // end of containos

#endif
