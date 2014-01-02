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
#ifndef containos_event_inl
#define containos_event_inl

namespace containos {

namespace internal {

template<typename E>
struct EventBase
{
protected:
    __forceinline EventBase()
        : m_delegates()
    {
    }

    template<typename funcptr_t>
    __forceinline void subscribe(funcptr_t func)
    {
        E::DelegateType newDelegate(func);
        m_delegates.insert(newDelegate.GetMemento());
    }

    template<typename T,typename funcptr_t>
    __forceinline void subscribe(T* ptr, funcptr_t func)
    {
        E::DelegateType newDelegate(ptr, func);
        m_delegates.insert(newDelegate.GetMemento());
    }

    template<typename T,typename funcptr_t>
    __forceinline void unsubscribe(T* ptr, funcptr_t func)
    {
        //E::DelegateType newDelegate(ptr, func);
        //m_delegates.insert(newDelegate.GetMemento());
    }

    List<fastdelegate::DelegateMemento,ListGrowRule<4> > m_delegates;
};

} // end of internal
//-----------------------------------------------------------------------------

template<typename R>
__forceinline Event<R()>::Event() : EventBase<Event<R()> >()
{
}

template<typename R>
__forceinline bool Event<R()>::isValid() const
{
    return m_delegates.size() > 0;
}

template<typename R>
__forceinline R Event<R()>::operator()()
{
    DelegateType invoker;
    for(size_t i = 0; i < m_delegates.size(); ++i) {
        invoker.SetMemento(m_delegates[i]);
        invoker();
    }
}

template<typename R>
template<typename funcptr_t>
__forceinline void Event<R()>::add(funcptr_t func)
{
    subscribe(func);
}

template<typename R>
template<typename T,typename funcptr_t>
__forceinline void Event<R()>::add(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A>
__forceinline Event<R(A)>::Event() : EventBase<Event<R(A)> >()
{
}

template<typename R,typename A>
__forceinline bool Event<R(A)>::isValid() const
{
    return m_delegates.size() > 0;
}

template<typename R,typename A>
__forceinline R Event<R(A)>::operator()(A a)
{
    DelegateType invoker;
    for(size_t i = 0; i < m_delegates.size(); ++i) {
        invoker.SetMemento(m_delegates[i]);
        invoker(a);
    }
}

template<typename R,typename A>
template<typename funcptr_t>
__forceinline void Event<R(A)>::add(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A>
template<typename T,typename funcptr_t>
__forceinline void Event<R(A)>::add(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B>
__forceinline Event<R(A,B)>::Event() : EventBase<Event<R(A,B)> >()
{
}

template<typename R,typename A,typename B>
__forceinline bool Event<R(A,B)>::isValid() const
{
    return m_delegates.size() > 0;
}

template<typename R,typename A,typename B>
__forceinline R Event<R(A,B)>::operator()(A a, B b)
{
    DelegateType invoker;
    for(size_t i = 0; i < m_delegates.size(); ++i) {
        invoker.SetMemento(m_delegates[i]);
        invoker(a, b);
    }
}

template<typename R,typename A,typename B>
template<typename funcptr_t>
__forceinline void Event<R(A,B)>::add(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A,typename B>
template<typename T,typename funcptr_t>
__forceinline void Event<R(A,B)>::add(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B,typename C>
__forceinline Event<R(A,B,C)>::Event() : EventBase<Event<R(A,B,C)> >()
{
}

template<typename R,typename A,typename B,typename C>
__forceinline bool Event<R(A,B,C)>::isValid() const
{
    return m_delegates.size() > 0;
}

template<typename R,typename A,typename B,typename C>
__forceinline R Event<R(A,B,C)>::operator()(A a, B b, C c)
{
    DelegateType invoker;
    for(size_t i = 0; i < m_delegates.size(); ++i) {
        invoker.SetMemento(m_delegates[i]);
        invoker(a, b, c);
    }
}

template<typename R,typename A,typename B,typename C>
template<typename funcptr_t>
__forceinline void Event<R(A,B,C)>::add(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A,typename B,typename C>
template<typename T,typename funcptr_t>
__forceinline void Event<R(A,B,C)>::add(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B,typename C,typename D>
__forceinline Event<R(A,B,C,D)>::Event() : EventBase<Event<R(A,B,C,D)> >()
{
}

template<typename R,typename A,typename B,typename C,typename D>
__forceinline bool Event<R(A,B,C,D)>::isValid() const
{
    return m_delegates.size() > 0;
}

template<typename R,typename A,typename B,typename C,typename D>
__forceinline R Event<R(A,B,C,D)>::operator()(A a, B b, C c, D d)
{
    DelegateType invoker;
    for(size_t i = 0; i < m_delegates.size(); ++i) {
        invoker.SetMemento(m_delegates[i]);
        invoker(a, b, c, d);
    }
}

template<typename R,typename A,typename B,typename C,typename D>
template<typename funcptr_t>
__forceinline void Event<R(A,B,C,D)>::add(funcptr_t func)
{
    subscribe(func);
}

template<typename R,typename A,typename B,typename C,typename D>
template<typename T,typename funcptr_t>
__forceinline void Event<R(A,B,C,D)>::add(T* ptr, funcptr_t func)
{
    subscribe(ptr, func);
}

} // end of containos

#endif
