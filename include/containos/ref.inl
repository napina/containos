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
#ifndef containos_ref_inl
#define containos_ref_inl

namespace containos {

template<typename T>
__forceinline Ref<T>::~Ref()
{
    removeRef();
}

template<typename T>
__forceinline Ref<T>::Ref()
    : m_mem(nullptr)
{
}

template<typename T>
__forceinline Ref<T>::Ref(nullptr_t)
    : m_mem(nullptr)
{
}

template<typename T>
__forceinline Ref<T>::Ref(T const* ptr)
{
    set(ptr);
}

template<typename T>
__forceinline Ref<T>::Ref(Ref<T> const& handle)
{
    set(handle.m_mem);
}

template<typename T>
__forceinline Ref<T>& Ref<T>::operator=(T const* ptr)
{
    reset(ptr);
    return *this;
}

template<typename T>
__forceinline Ref<T>& Ref<T>::operator=(Ref<T> const& handle)
{
    reset(handle.m_mem);
    return *this;
}

template<typename T>
__forceinline Ref<T>& Ref<T>::operator=(nullptr_t)
{
    removeRef();
    return *this;
}

template<typename T>
__forceinline bool Ref<T>::operator==(Ref<T> const& other) const
{
    return (m_mem == other.m_mem);
}

template<typename T>
__forceinline bool Ref<T>::operator!=(Ref<T> const& other) const
{
    return (m_mem != other.m_mem);
}

template<typename T>
__forceinline bool Ref<T>::operator==(T const* other) const
{
    return (m_mem == other);
}

template<typename T>
__forceinline bool Ref<T>::operator!=(T const* other) const
{
    return (m_mem != other);
}

template<typename T>
__forceinline bool Ref<T>::operator<(Ref<T> const& other) const
{
    return m_mem < other.m_mem;
}

template<typename T>
__forceinline Ref<T>::operator T*() const
{
    return m_mem;
}

template<typename T>
__forceinline Ref<T>::operator T const*() const
{
    return m_mem;
}

template<typename T>
__forceinline T const& Ref<T>::operator*() const
{
    return *m_mem;
}

template<typename T>
__forceinline T& Ref<T>::operator*()
{
    return *m_mem;
}

template<typename T>
__forceinline T const* Ref<T>::operator->() const
{
    return m_mem;
}

template<typename T>
__forceinline T* Ref<T>::operator->()
{
    return m_mem;
}

template<typename T>
__forceinline T const* Ref<T>::get() const
{
    return m_mem;
}

template<typename T>
__forceinline T* Ref<T>::get()
{
    return m_mem;
}

template<typename T>
__forceinline bool Ref<T>::isValid() const
{
    return m_mem != nullptr;
}

template<typename T>
template<typename T2>
__forceinline T2* Ref<T>::cast()
{
    return containos_cast(T2*, m_mem);
}

template<typename T>
__forceinline void Ref<T>::reset(T const* ptr)
{
    removeRef();
    set(ptr);
}

template<typename T>
__forceinline T* Ref<T>::release()
{
    T* releasedPtr = m_mem;
    set(nullptr);
    return releasedPtr;
}

template<typename T>
__forceinline void Ref<T>::set(T const* ptr)
{
    m_mem = const_cast<T*>(ptr);
    if(m_mem) {
        ++(m_mem->m_refCount);
    }
}

template<typename T>
__forceinline void Ref<T>::removeRef()
{
    if(m_mem) {
        --(m_mem->m_refCount);
        if(m_mem->m_refCount == 0) {
            containos_delete(m_mem);
        }
        m_mem = nullptr;
    }
}

} // end of containos

#endif
