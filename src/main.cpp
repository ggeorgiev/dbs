#include "err/err.h"

#include <iostream>

ECode test0()
{
    EHRET(kError);
}

ECode test1()
{
    int a = 5;
    EHRET(kError, a);
}

ECode test2()
{
    int a = 5;
    int b = 5;
    EHRET(kError, a, b);
}

ECode test3()
{
    int a = 5;
    int b = 5;
    int c = 5;
    EHRET(kError, a, b, c);
}

ECode test4()
{
    int a = 5;
    int b = 5;
    EHRET(kError, a, b, a, b);
}

int main(int argc, const char* argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    test0();
    test1();
    test2();
    test3();
    test4();
    return 0;
}
