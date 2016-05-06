//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "rtti/class_rtti.hpp"
#include "gtest/framework.h"

namespace rtti
{
class Class1 : public RttiInfo<Class1>
{
};

class Class2 : public RttiInfo<Class2>
{
};

TEST(RttiClass, ClassId)
{
    ASSERT_NE(Class1::classId(), Class2::classId());
}

template <typename T>
class Base : public RttiInfo<T>
{
};

class IndirectClass1 : public Base<IndirectClass1>
{
};

class IndirectClass2 : public Base<IndirectClass2>
{
};

TEST(RttiClass, IndirectClass)
{
    ASSERT_NE(IndirectClass1::classId(), IndirectClass2::classId());
}
}