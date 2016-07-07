//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

namespace rtti
{
typedef const void* ClassId;

template <typename T, int N = 0>
class RttiInfo
{
public:
    static ClassId classId()
    {
        return static_cast<ClassId>(&ID);
    }

private:
    static char ID;
};

template <typename T, int N>
char RttiInfo<T, N>::ID = 0;
}
