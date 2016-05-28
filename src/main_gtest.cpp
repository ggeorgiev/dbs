//  Copyright © 2016 George Georgiev. All rights reserved.
//

namespace testing
{
extern int run(int argc, char* argv[]);
}

int main(int argc, char* argv[])
{
    return testing::run(argc, argv);
    // return 0;
}
