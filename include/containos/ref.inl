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

#define REF_STORAGE_IMPL(ClassType,SizeType)\
    friend struct containos::internal::modify_reference<ClassType,false>;\
    mutable SizeType m_refCount

#define REF_STORAGE_BITS_IMPL(ClassType,SizeType,Bits)\
    friend struct containos::internal::modify_reference<ClassType,false>;\
    mutable SizeType m_refCount : Bits

#define REF_DERIVED_IMPL(ClassType)\
    friend struct containos::internal::modify_reference<ClassType,false>;

#define REF_STORAGE_INIT_IMPL()\
    m_refCount(0)

#define REF_STORAGE_RESET_IMPL()\
    m_refCount = 0

namespace containos {
namespace internal {
    struct true_type  { enum { value = true  }; };
    struct false_type { enum { value = false }; };

    template<typename T>
    struct has_reference_override
    {
        template<typename C> static true_type test(decltype(&T::addReference));
        template<typename C> static false_type test(...);
        typedef decltype(test<T>(nullptr)) type;
        static const bool value = type::value;
    };

    template<typename T,bool Override>
    struct modify_reference
    {
        inline static void add(T* ptr)         { ++(ptr->m_refCount); }
        inline static uint32_t remove(T* ptr)  { return --(ptr->m_refCount); }
    };

    template<typename T>
    struct modify_reference<T,true>
    {
        inline static void add(T* ptr)         { ptr->addReference(); }
        inline static uint32_t remove(T* ptr)  { return ptr->removeReference(); }
    };
}

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
__forceinline Ref<T>::Ref(Ref<T> const& other)
{
    set(other.m_mem);
}

template<typename T>
__forceinline Ref<T>& Ref<T>::operator=(T const* ptr)
{
    reset(ptr);
    return *this;
}

template<typename T>
__forceinline Ref<T>& Ref<T>::operator=(Ref<T> const& other)
{
    reset(other.m_mem);
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
__forceinline Ref<T>::operator bool() const
{
    return m_mem != nullptr;
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
__forceinline void Ref<T>::reset()
{
    removeRef();
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
        typedef internal::has_reference_override<T> has_override;
        internal::modify_reference<T,has_override::value>::add(m_mem);
    }
}

template<typename T>
__forceinline void Ref<T>::removeRef()
{
    if(m_mem) {
        typedef internal::has_reference_override<T> has_override;
        uint32_t newCount = internal::modify_reference<T,has_override::value>::remove(m_mem);
        if(newCount == 0) {
            containos_delete(m_mem);
        }
        m_mem = nullptr;
    }
}

} // end of containos

#endif
