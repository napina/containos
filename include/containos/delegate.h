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
#ifndef containos_delegate_h
#define containos_delegate_h

#include "containos/common.h"
#include "containos/allocator.h"
#include "containos/fastdelegate.h"

namespace containos {
namespace internal {
    template<typename D> struct DelegateBase;
}

template<typename Signature>
struct Delegate;
//-----------------------------------------------------------------------------

template<typename R>
struct Delegate<R()> : private internal::DelegateBase<Delegate<R()> >
{
    typedef fastdelegate::FastDelegate0<R> DelegateType;

    Delegate();
    Delegate(Delegate&& other);
    ~Delegate();
    R operator()();
    bool isValid() const;
    template<typename lambda_t>             void set(const lambda_t& lambda, Allocator* allocator);
    template<typename funcptr_t>            void set(funcptr_t func);
    template<typename T,typename funcptr_t> void set(T* ptr, funcptr_t func);
};
//-----------------------------------------------------------------------------

template<typename R,typename A>
struct Delegate<R(A)> : private internal::DelegateBase<Delegate<R(A)> >
{
    typedef fastdelegate::FastDelegate1<A,R> DelegateType;

    Delegate();
    Delegate(Delegate&& other);
    ~Delegate();
    R operator()(A a);
    bool isValid() const;
    template<typename lambda_t>             void set(const lambda_t& func, Allocator* allocator);
    template<typename funcptr_t>            void set(funcptr_t func);
    template<typename T,typename funcptr_t> void set(T* ptr, funcptr_t func);
};
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B>
struct Delegate<R(A,B)> : private internal::DelegateBase<Delegate<R(A,B)> >
{
    typedef fastdelegate::FastDelegate2<A,B,R> DelegateType;

    Delegate();
    Delegate(Delegate&& other);
    ~Delegate();
    R operator()(A a, B b);
    bool isValid() const;
    template<typename lambda_t>             void set(const lambda_t& func, Allocator* allocator);
    template<typename funcptr_t>            void set(funcptr_t func);
    template<typename T,typename funcptr_t> void set(T* ptr, funcptr_t func);
};
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B, typename C>
struct Delegate<R(A,B,C)> : private internal::DelegateBase<Delegate<R(A,B,C)> >
{
    typedef fastdelegate::FastDelegate3<A,B,C,R> DelegateType;

    Delegate();
    Delegate(Delegate&& other);
    ~Delegate();
    R operator()(A a, B b, C c);
    bool isValid() const;
    template<typename lambda_t>             void set(const lambda_t& func, Allocator* allocator);
    template<typename funcptr_t>            void set(funcptr_t func);
    template<typename T,typename funcptr_t> void set(T* ptr, funcptr_t func);
};
//-----------------------------------------------------------------------------

template<typename R,typename A,typename B, typename C,typename D>
struct Delegate<R(A,B,C,D)> : private internal::DelegateBase<Delegate<R(A,B,C,D)> >
{
    typedef fastdelegate::FastDelegate4<A,B,C,D,R> DelegateType;

    Delegate();
    Delegate(Delegate&& other);
    ~Delegate();
    R operator()(A a, B b, C c, D d);
    bool isValid() const;
    template<typename lambda_t>             void set(const lambda_t& func, Allocator* allocator);
    template<typename funcptr_t>            void set(funcptr_t func);
    template<typename T,typename funcptr_t> void set(T* ptr, funcptr_t func);
};
//-----------------------------------------------------------------------------

} // end of containos

#include "containos/delegate.inl"

#endif
