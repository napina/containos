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
#ifndef containos_delegate_inl
#define containos_delegate_inl

namespace containos {

namespace internal {

struct DelegateMemento : public fastdelegate::DelegateMemento
{
    __forceinline void* getThis()
    {
        return m_pthis;
    }

    __forceinline bool isValid() const
    {
        return !empty();
    }

    __forceinline void set(const fastdelegate::DelegateMemento& memento)
    {
        SetMementoFrom(memento);
    }
};

template<typename D>
struct DelegateBase
{
protected:
    __forceinline DelegateBase()
        : m_memento()
        , m_delete(nullptr)
        , m_allocator(nullptr)
    {
    }

    template<typename lambda_t>
    __forceinline void subscribe(const lambda_t& lambda, Allocator* allocator)
    {
        lambda_t* copy = reinterpret_cast<lambda_t*>(allocator->alloc(sizeof(lambda_t),std::alignment_of<lambda_t>::value));
        containos_placement_new1(copy, lambda_t, lambda);
        D::DelegateType newDelegate(copy, &lambda_t::operator());
        m_memento.set(newDelegate.GetMemento());
        m_delete = [](void* ptr) { containos_placement_delete(ptr, lambda_t); };
        m_allocator = allocator;
    }

    template<typename funcptr_t>
    __forceinline void subscribe(funcptr_t func)
    {
        D::DelegateType newDelegate(func);
        m_memento.set(newDelegate.GetMemento());
    }

    template<typename T,typename funcptr_t>
    __forceinline void subscribe(T* ptr, funcptr_t func)
    {
        D::DelegateType newDelegate(ptr, func);
        m_memento.set(newDelegate.GetMemento());
    }

    __forceinline void unsubscribe()
    {
        if(m_delete) {
            m_delete(m_memento.getThis());
            m_delete = nullptr;
            m_allocator->dealloc(m_memento.getThis());
            m_allocator = nullptr;
        }
        m_memento.clear();
    }

    DelegateMemento m_memento;
    void (*m_delete)(void*);
    Allocator* m_allocator;
};

} // end of internal
//-----------------------------------------------------------------------------

template<typename R>
__forceinline Delegate<R()>::Delegate()
    : internal::DelegateBase<Delegate<R()> >()
{
}
/*
template<typename R>
__forceinline Delegate<R()>::Delegate()
    : internal::DelegateBase<Delegate<R()> >(Delegate&& other)
{
    m_memento = other.m_memento;
    m_delete = other.m_delete;
    m_allocator = other.allocator;
}
*/
template<typename R>
__forceinline Delegate<R()>::~Delegate()
{
    unsubscribe();
}

template<typename R>
__forceinline bool Delegate<R()>::isValid() const
{
    return m_memento.isValid();
}

template<typename R>
__forceinline R Delegate<R()>::operator()()
{
    DelegateType invoker;
    invoker.SetMemento(m_memento);
    return invoker();
}

template<typename R>
template<typename lambda_t>
__forceinline void Delegate<R()>::set(const lambda_t& lambda, Allocator* allocator)
{
    subscribe(lambda, allocator);
}

template<typename R>
template<typename funcptr_t>
__forceinline void Delegate<R()>::set(funcptr_t func)
{
    subscribe(func);
}

template<typename R>
template<typename T,typename funcptr_t>
__forceinline void Delegate<R()>::set(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A>
__forceinline Delegate<R(A)>::Delegate()
    : internal::DelegateBase<Delegate<R(A)> >()
{
}

template<typename R,typename A>
__forceinline Delegate<R(A)>::~Delegate()
{
    unsubscribe();
}

template<typename R,typename A>
__forceinline bool Delegate<R(A)>::isValid() const
{
    return m_memento.isValid();
}

template<typename R,typename A>
__forceinline R Delegate<R(A)>::operator()(A a)
{
    DelegateType invoker;
    invoker.SetMemento(m_memento);
    return invoker(a);
}

template<typename R,typename A>
template<typename lambda_t>
__forceinline void Delegate<R(A)>::set(const lambda_t& lambda, Allocator* allocator)
{
    subscribe(lambda, allocator);
}

template<typename R,typename A>
template<typename funcptr_t>
__forceinline void Delegate<R(A)>::set(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A>
template<typename T,typename funcptr_t>
__forceinline void Delegate<R(A)>::set(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B>
__forceinline Delegate<R(A,B)>::Delegate()
    : internal::DelegateBase<Delegate<R(A,B)> >()
{
}

template<typename R,typename A,typename B>
__forceinline Delegate<R(A,B)>::~Delegate()
{
    unsubscribe();
}

template<typename R,typename A,typename B>
__forceinline bool Delegate<R(A,B)>::isValid() const
{
    return m_memento.isValid();
}

template<typename R,typename A,typename B>
__forceinline R Delegate<R(A,B)>::operator()(A a, B b)
{
    DelegateType invoker;
    invoker.SetMemento(m_memento);
    return invoker(a, b);
}

template<typename R,typename A,typename B>
template<typename lambda_t>
__forceinline void Delegate<R(A,B)>::set(const lambda_t& lambda, Allocator* allocator)
{
    subscribe(lambda, allocator);
}

template<typename R,typename A,typename B>
template<typename funcptr_t>
__forceinline void Delegate<R(A,B)>::set(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A,typename B>
template<typename T,typename funcptr_t>
__forceinline void Delegate<R(A,B)>::set(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B,typename C>
__forceinline Delegate<R(A,B,C)>::Delegate()
    : internal::DelegateBase<Delegate<R(A,B,C)> >()
{
}

template<typename R,typename A,typename B,typename C>
__forceinline Delegate<R(A,B,C)>::~Delegate()
{
    unsubscribe();
}

template<typename R,typename A,typename B,typename C>
__forceinline bool Delegate<R(A,B,C)>::isValid() const
{
    return m_memento.isValid();
}

template<typename R,typename A,typename B,typename C>
__forceinline R Delegate<R(A,B,C)>::operator()(A a, B b, C c)
{
    DelegateType invoker;
    invoker.SetMemento(m_memento);
    return invoker(a, b, c);
}

template<typename R,typename A,typename B,typename C>
template<typename lambda_t>
__forceinline void Delegate<R(A,B,C)>::set(const lambda_t& lambda, Allocator* allocator)
{
    subscribe(lambda, allocator);
}

template<typename R,typename A,typename B,typename C>
template<typename funcptr_t>
__forceinline void Delegate<R(A,B,C)>::set(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A,typename B,typename C>
template<typename T,typename funcptr_t>
__forceinline void Delegate<R(A,B,C)>::set(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B,typename C,typename D>
__forceinline Delegate<R(A,B,C,D)>::Delegate()
    : internal::DelegateBase<Delegate<R(A,B,C,D)> >()
{
}

template<typename R,typename A,typename B,typename C,typename D>
__forceinline Delegate<R(A,B,C,D)>::~Delegate()
{
    unsubscribe();
}

template<typename R,typename A,typename B,typename C,typename D>
__forceinline bool Delegate<R(A,B,C,D)>::isValid() const
{
    return m_memento.isValid();
}

template<typename R,typename A,typename B,typename C,typename D>
__forceinline R Delegate<R(A,B,C,D)>::operator()(A a, B b, C c, D d)
{
    DelegateType invoker;
    invoker.SetMemento(m_memento);
    return invoker(a, b, c, d);
}

template<typename R,typename A,typename B,typename C,typename D>
template<typename lambda_t>
__forceinline void Delegate<R(A,B,C,D)>::set(const lambda_t& lambda, Allocator* allocator)
{
    subscribe(lambda, allocator);
}

template<typename R,typename A,typename B,typename C,typename D>
template<typename funcptr_t>
__forceinline void Delegate<R(A,B,C,D)>::set(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A,typename B,typename C,typename D>
template<typename T,typename funcptr_t>
__forceinline void Delegate<R(A,B,C,D)>::set(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}

} // end of containos

#endif
