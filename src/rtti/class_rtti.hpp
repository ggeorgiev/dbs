//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

namespace rtti
{
typedef const void* ClassId;

template <typename T>
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

template <typename T>
char RttiInfo<T>::ID = 0;
}
