//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <iosfwd>
#include <string>

extern std::string kEmptyString;
extern const char* kEmptyStringLiteral;

extern std::string kCurrentDirectoryString;
extern std::string kParentDirectoryString;

inline char slash()
{
    return '/';
}
