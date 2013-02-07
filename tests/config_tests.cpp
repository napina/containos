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
#include "containos/config.h"

namespace c = containos;

namespace {
struct IntWrap
{
    IntWrap()                      { m_mem = new int(0); }
    IntWrap(int i)                 { m_mem = new int(i); }
    IntWrap(IntWrap const& other)  { m_mem = new int(*other.m_mem); }
    ~IntWrap()                     { delete m_mem; }
    operator int() const           { return *m_mem; }
    int& operator=(int i)          { (*m_mem) = i; return *m_mem; }
    bool operator==(int i) const   { return *m_mem == i; }
private:
    int* m_mem; 
};
}

TEST_SUITE(Config)
{
    TEST(NewDeleteInt)
    {
        char mem[sizeof(int)];
        int* ptr = containos_placement_new(mem, int);
        *ptr = 13;
        EXPECT_EQUAL(*ptr, 13);
        containos_placement_delete(ptr, int);
    }

    TEST(CopyInt)
    {
        char mem[sizeof(int)];
        int other(12);
        int* ptr = containos_placement_copy(mem, int, other);
        EXPECT_EQUAL(*ptr, 12);
        containos_placement_delete(ptr, int);
    }

    TEST(NewDeleteIntWrap)
    {
        char mem[sizeof(IntWrap)];
        IntWrap* ptr = containos_placement_new1(mem, IntWrap, 13);
        EXPECT_EQUAL(*ptr, 13);
        containos_placement_delete(ptr, IntWrap);
    }

    TEST(CopyIntWrap)
    {
        char mem[sizeof(IntWrap)];
        IntWrap other(12);
        IntWrap* ptr = containos_placement_copy(mem, IntWrap, other);
        EXPECT_EQUAL(*ptr, 12);
        containos_placement_delete(ptr, IntWrap);
    }
}
