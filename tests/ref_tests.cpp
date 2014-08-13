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
#include "containos/ref.h"
#include <memory>

namespace c = containos;

namespace {

struct BaseClass
{
    virtual ~BaseClass() {}

    BaseClass()
        : m_value(0)
        , REF_STORAGE_INIT()
    {
    }

protected:
    int m_value;
    REF_STORAGE(BaseClass,int);
};

struct DerivedClass : public BaseClass
{
    virtual ~DerivedClass() {
        delete m_ptr;
    }

    DerivedClass() : BaseClass() {
        m_ptr = new int(33);
    }

private:
    int* m_ptr;
    REF_DERIVED(DerivedClass);
};

struct OverrideClass
{
    OverrideClass()
        : m_count(0)
    {
    }

    void addReference()         { ++m_count; }
    uint32_t removeReference()  { return --m_count; }
    uint32_t count()            { return m_count; }

protected:
    int m_count;
};

}

TEST_SUITE(Ref)
{
    TEST(Invalid)
    {
        c::Ref<BaseClass> ptr;
        EXPECT_FALSE(ptr.isValid());
        EXPECT_TRUE(ptr == nullptr);
        EXPECT_FALSE(ptr != nullptr);
        EXPECT_TRUE(ptr.get() == nullptr);
    }

    TEST(CreateAutoDelete)
    {
        c::Ref<BaseClass> ptr;
        ptr = new DerivedClass();
        EXPECT_TRUE(ptr.isValid());
        EXPECT_FALSE(ptr == nullptr);
        EXPECT_TRUE(ptr != nullptr);
        EXPECT_FALSE(ptr.get() == nullptr);
    }

    TEST(CreateManualDelete)
    {
        c::Ref<BaseClass> ptr;
        ptr = new DerivedClass();
        EXPECT_TRUE(ptr.isValid());
        EXPECT_FALSE(ptr == nullptr);
        EXPECT_TRUE(ptr != nullptr);
        EXPECT_FALSE(ptr.get() == nullptr);
        ptr = nullptr;
        EXPECT_FALSE(ptr.isValid());
    }

    TEST(CreateDerived)
    {
        c::Ref<DerivedClass> ptr;
        ptr = new DerivedClass();
        EXPECT_TRUE(ptr.isValid());
        EXPECT_FALSE(ptr == nullptr);
        EXPECT_TRUE(ptr != nullptr);
        EXPECT_FALSE(ptr.get() == nullptr);
        ptr = nullptr;
        EXPECT_FALSE(ptr.isValid());
    }

    TEST(CreateOverride)
    {
        c::Ref<OverrideClass> ptr;
        ptr = new OverrideClass();
        EXPECT_TRUE(ptr.isValid());
        EXPECT_FALSE(ptr == nullptr);
        EXPECT_TRUE(ptr != nullptr);
        EXPECT_FALSE(ptr.get() == nullptr);
        EXPECT_EQUAL(ptr->count(), 1u);
        ptr = nullptr;
        EXPECT_FALSE(ptr.isValid());
    }
}
