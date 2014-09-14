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

__forceinline UndoList::~UndoList()
{
    clear();
}

__forceinline UndoList::UndoList(size_t capacity)
    : m_commandStack(capacity)
    , m_currentIndex(0)
{
}

template<typename T>
__forceinline void UndoList::add()
{
    clearFuture();
    T* command = (T*)m_commandStack.allocator()->alloc(sizeof(T), __alignof(T));
    containos_placement_new(command, T);
    m_commandStack.insert(command);
    ++m_currentIndex;
}

template<typename T,typename A>
__forceinline void UndoList::add(A a)
{
    clearFuture();
    T* command = (T*)m_commandStack.allocator()->alloc(sizeof(T), __alignof(T));
    containos_placement_new1(command, T, a);
    m_commandStack.insert(command);
    ++m_currentIndex;
}

template<typename T,typename A,typename B>
__forceinline void UndoList::add(A a, B b)
{
    clearFuture();
    T* command = (T*)m_commandStack.allocator()->alloc(sizeof(T), __alignof(T));
    containos_placement_new2(command, T, a, b);
    m_commandStack.insert(command);
    ++m_currentIndex;
}

template<typename T,typename A,typename B,typename C>
__forceinline void UndoList::add(A a, B b, C c)
{
    clearFuture();
    T* command = (T*)m_commandStack.allocator()->alloc(sizeof(T), __alignof(T));
    containos_placement_new3(command, T, a, b, c);
    m_commandStack.insert(command);
    ++m_currentIndex;
}

template<typename T,typename A,typename B,typename C,typename D>
__forceinline void UndoList::add(A a, B b, C c, D d)
{
    clearFuture();
    T* command = (T*)m_commandStack.allocator()->alloc(sizeof(T), __alignof(T));
    containos_placement_new4(command, T, a, b, c, d);
    m_commandStack.insert(command);
    ++m_currentIndex;
}

__forceinline void UndoList::undo(size_t count)
{
    containos_assert(count <= undoCount());
    for(size_t i = 0; i < count; ++i) {
        --m_currentIndex;
        m_commandStack[m_currentIndex]->reverse();
    }
}

__forceinline void UndoList::redo(size_t count)
{
    containos_assert(count <= redoCount());
    for(size_t i = 0; i < count; ++i) {
        m_commandStack[m_currentIndex]->reverse();
        ++m_currentIndex;
    }
}

inline void UndoList::clearHistory()
{
    size_t removeCount = undoCount();
    size_t newSize = redoCount();
    for(size_t i = 0; i < removeCount; ++i) {
        containos_placement_delete(m_commandStack[i], UndoCommand);
        m_commandStack.allocator()->dealloc(m_commandStack[i]);
    }
    for(size_t i = 0; i < newSize; ++i) {
        m_commandStack[i] = m_commandStack[removeCount + i];
    }
    m_commandStack.resize(newSize);
    m_currentIndex = 0;
}

inline void UndoList::clearFuture()
{
    size_t oldSize = m_commandStack.size();
    for(size_t i = undoCount(); i < oldSize; ++i) {
        containos_placement_delete(m_commandStack[i], UndoCommand);
        m_commandStack.allocator()->dealloc(m_commandStack[i]);
    }
    m_commandStack.resize(undoCount());
}

inline void UndoList::clear()
{
    CommandStack::iterator ite = m_commandStack.begin();
    CommandStack::iterator endIte = m_commandStack.end();
    for(; ite != endIte; ++ite) {
        containos_placement_delete(*ite, UndoCommand);
        m_commandStack.allocator()->dealloc(*ite);
    }
    m_commandStack.clear();
    m_currentIndex = 0;
}

__forceinline size_t UndoList::undoCount() const
{
    return m_currentIndex;
}

__forceinline size_t UndoList::redoCount() const
{
    return m_commandStack.size() - m_currentIndex;
}

__forceinline bool UndoList::isUndoAllowed() const
{
    return undoCount() > 0;
}

__forceinline bool UndoList::isRedoAllowed() const
{
    return redoCount() > 0;
}

} // end of containos

#endif
