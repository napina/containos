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
#ifndef containos_undolist_inl
#define containos_undolist_inl

namespace containos {

template<typename Allocator>
__forceinline UndoList<Allocator>::~UndoList()
{
    clear();
}

template<typename Allocator>
__forceinline UndoList<Allocator>::UndoList(size_t capacity)
    : Base()
    , m_commandStack(capacity)
    , m_currentIndex(0)
{
}

template<typename Allocator>
template<typename T>
__forceinline void UndoList<Allocator>::add()
{
    clearFuture();
    m_commandStack.insert(Base::template construct<T>());
    ++m_currentIndex;
}

template<typename Allocator>
template<typename T, typename ARG1>
__forceinline void UndoList<Allocator>::add(ARG1 arg1)
{
    clearFuture();
    m_commandStack.insert(Base::template construct<T>(arg1));
    ++m_currentIndex;
}

template<typename Allocator>
template<typename T, typename ARG1, typename ARG2>
__forceinline void UndoList<Allocator>::add(ARG1 arg1, ARG2 arg2)
{
    clearFuture();
    m_commandStack.insert(Base::template construct<T>(arg1, arg2));
    ++m_currentIndex;
}

template<typename Allocator>
template<typename T, typename ARG1, typename ARG2, typename ARG3>
__forceinline void UndoList<Allocator>::add(ARG1 arg1, ARG2 arg2, ARG3 arg3)
{
    clearFuture();
    m_commandStack.insert(Base::template construct<T>(arg1, arg2, arg3));
    ++m_currentIndex;
}

template<typename Allocator>
template<typename T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
__forceinline void UndoList<Allocator>::add(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4)
{
    clearFuture();
    m_commandStack.insert(Base::template construct<T>(arg1, arg2, arg3, arg4));
    ++m_currentIndex;
}

template<typename Allocator>
__forceinline void UndoList<Allocator>::undo(size_t count)
{
    containos_assert(count <= undoCount());
    for(size_t i = 0; i < count; ++i) {
        --m_currentIndex;
        m_commandStack[m_currentIndex]->reverse();
    }
}

template<typename Allocator>
__forceinline void UndoList<Allocator>::redo(size_t count)
{
    containos_assert(count <= redoCount());
    for(size_t i = 0; i < count; ++i) {
        m_commandStack[m_currentIndex]->reverse();
        ++m_currentIndex;
    }
}

template<typename Allocator>
inline void UndoList<Allocator>::clearHistory()
{
    size_t removeCount = undoCount();
    size_t newSize = redoCount();
    for(size_t i = 0; i < removeCount; ++i) {
        Base::template destruct<UndoCommand>(m_commandStack[i]);
    }
    for(size_t i = 0; i < newSize; ++i) {
        m_commandStack[i] = m_commandStack[removeCount + i];
    }
    m_commandStack.resize(newSize);
    m_currentIndex = 0;
}

template<typename Allocator>
inline void UndoList<Allocator>::clearFuture()
{
    size_t oldSize = m_commandStack.size();
    for(size_t i = undoCount(); i < oldSize; ++i) {
        Base::template destruct<UndoCommand>(m_commandStack[i]);
    }
    m_commandStack.resize(undoCount());
}

template<typename Allocator>
inline void UndoList<Allocator>::clear()
{
    CommandStack::iterator ite = m_commandStack.begin();
    CommandStack::iterator endIte = m_commandStack.end();
    for(; ite != endIte; ++ite) {
        Base::template destruct<UndoCommand>(*ite);
    }
    m_commandStack.clear();
    m_currentIndex = 0;
}

template<typename Allocator>
__forceinline size_t UndoList<Allocator>::undoCount() const
{
    return m_currentIndex;
}

template<typename Allocator>
__forceinline size_t UndoList<Allocator>::redoCount() const
{
    return m_commandStack.size() - m_currentIndex;
}

template<typename Allocator>
__forceinline bool UndoList<Allocator>::isUndoAllowed() const
{
    return undoCount() > 0;
}

template<typename Alloc>
__forceinline bool UndoList<Alloc>::isRedoAllowed() const
{
    return redoCount() > 0;
}

} // end of containos

#endif
