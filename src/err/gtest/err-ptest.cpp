#include "err/err.h"
#include "err/err_serialize.h"
#include "gtest/framework.h"
#include <cstdio>
#include <sstream>

struct SerialiazeBase
{
    SerialiazeBase()
        : mA(0)
        , mB("1010101")
        , mC(111)
        , mD("17777177771777717777")
    {
    }

protected:
    int mA;
    std::string mB;
    int mC;
    std::string mD;
};

struct SerialiazeStringStream : public SerialiazeBase
{
    void operator()()
    {
        mStream << EH_SERIALIZE(kExpected, mA, mB, mC, mD);
    }

private:
    std::stringstream mStream;
};

struct SerialiazeFormat : public SerialiazeBase
{
    void operator()()
    {
        mString += EH_CPPFORMAT(kExpected, mA, mB, mC, mD);
    }

private:
    std::string mString;
};

struct SerialiazePrintf : public SerialiazeBase
{
    void operator()()
    {
        char buffer[1024];
        sprintf(buffer,
                "kExpected(%d): mA = %d, mB = %s, mC = %d, mD = %s\n",
                err::kExpected,
                mA,
                mB.c_str(),
                mC,
                mD.c_str());
        mString += buffer;
    }

private:
    std::string mString;
};

template <typename T>
class SerialiazePerformanceTest : public ::testing::Test
{
public:
    typedef T Functor;
};

typedef ::testing::Types<SerialiazeStringStream,
                         // SerialiazeConcatenate,
                         SerialiazeFormat,
                         SerialiazePrintf>
    SerialiazePerformanceType;

TYPED_TEST_CASE(SerialiazePerformanceTest, SerialiazePerformanceType);

TYPED_TEST(SerialiazePerformanceTest, PERFORMANCE_CPU)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        typename TestFixture::Functor functor;
        for (size_t j = 0; j < 25; ++j)
            functor();
    }
}
