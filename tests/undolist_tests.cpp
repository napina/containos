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
#include "unitos/unitos.h"
#include "containos/undolist.h"

namespace c = containos;

class SetIntCommand : public c::UndoCommand
{
public:
    virtual ~SetIntCommand()
    {
    }

    SetIntCommand(int* target, int newValue)
        : m_target(target)
        , m_oldValue(*target)
    {
        *m_target = newValue;
    }

    virtual void reverse()
    {
        int value = *m_target;
        *m_target = m_oldValue;
        m_oldValue = value;
    }

private:
    int* m_target;
    int m_oldValue;
};

TEST_SUITE(UndoList)
{
    TEST(Undo)
    {
        int value = 100;
        c::UndoList<> list(10);
        EXPECT_EQUAL(list.undoCount(), 0u);
        EXPECT_EQUAL(list.redoCount(), 0u);
        list.add<SetIntCommand>(&value, 23);
        EXPECT_EQUAL(value, 23);
        EXPECT_EQUAL(list.undoCount(), 1u);
        EXPECT_EQUAL(list.redoCount(), 0u);
        list.undo();
        EXPECT_EQUAL(value, 100);
        EXPECT_EQUAL(list.undoCount(), 0u);
        EXPECT_EQUAL(list.redoCount(), 1u);
    }

    TEST(UndoRedo)
    {
        int value = 100;
        c::UndoList<> list(10);
        list.add<SetIntCommand>(&value, 23);
        EXPECT_EQUAL(list.undoCount(), 1u);
        EXPECT_EQUAL(list.redoCount(), 0u);
        EXPECT_EQUAL(value, 23);
        list.undo();
        EXPECT_EQUAL(list.undoCount(), 0u);
        EXPECT_EQUAL(list.redoCount(), 1u);
        EXPECT_EQUAL(value, 100);
        list.redo();
        EXPECT_EQUAL(value, 23);
        EXPECT_EQUAL(list.undoCount(), 1u);
        EXPECT_EQUAL(list.redoCount(), 0u);
    }

    TEST(UndoMultiple)
    {
        c::UndoList<> list(10);
        int value = 100;
        for(int i = 0; i < 8; ++i) {
            list.add<SetIntCommand>(&value, i);
        }
        EXPECT_EQUAL(value, 7);
        EXPECT_EQUAL(list.undoCount(), 8u);
        EXPECT_EQUAL(list.redoCount(), 0u);
        list.undo(4);
        EXPECT_EQUAL(value, 3);
        EXPECT_EQUAL(list.undoCount(), 4u);
        EXPECT_EQUAL(list.redoCount(), 4u);
    }

    TEST(Clear)
    {
        int value = 100;
        c::UndoList<> list(10);
        list.add<SetIntCommand>(&value, 23);
        list.clear();
        EXPECT_EQUAL(list.undoCount(), 0u);
        EXPECT_EQUAL(list.redoCount(), 0u);
        EXPECT_EQUAL(value, 23);
    }

    TEST(ClearHistory)
    {
        c::UndoList<> list(10);
        int value = 100;
        for(int i = 0; i < 8; ++i) {
            list.add<SetIntCommand>(&value, i);
        }
        EXPECT_EQUAL(value, 7);
        list.undo(4);
        EXPECT_EQUAL(value, 3);
        EXPECT_EQUAL(list.undoCount(), 4u);
        EXPECT_EQUAL(list.redoCount(), 4u);
        list.clearHistory();
        EXPECT_EQUAL(list.undoCount(), 0u);
        EXPECT_EQUAL(list.redoCount(), 4u);
    }

    TEST(ClearFuture)
    {
        c::UndoList<> list(10);
        int value = 100;
        for(int i = 0; i < 8; ++i) {
            list.add<SetIntCommand>(&value, i);
        }
        EXPECT_EQUAL(value, 7);
        list.undo(4);
        EXPECT_EQUAL(value, 3);
        EXPECT_EQUAL(list.undoCount(), 4u);
        EXPECT_EQUAL(list.redoCount(), 4u);
        list.clearFuture();
        EXPECT_EQUAL(list.undoCount(), 4u);
        EXPECT_EQUAL(list.redoCount(), 0u);
    }

    TEST(UndoMultipleAndDo)
    {
        c::UndoList<> list(10);
        int value = 100;
        for(int i = 0; i < 8; ++i) {
            list.add<SetIntCommand>(&value, i);
        }
        EXPECT_EQUAL(value, 7);
        EXPECT_EQUAL(list.undoCount(), 8u);
        EXPECT_EQUAL(list.redoCount(), 0u);
        list.undo(4);
        EXPECT_EQUAL(value, 3);
        EXPECT_EQUAL(list.undoCount(), 4u);
        EXPECT_EQUAL(list.redoCount(), 4u);
        list.add<SetIntCommand>(&value, 50);
        EXPECT_EQUAL(value, 50);
        EXPECT_EQUAL(list.undoCount(), 5u);
        EXPECT_EQUAL(list.redoCount(), 0u);
    }
}
