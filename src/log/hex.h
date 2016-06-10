//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include <sstream>
#include <string>
#include <string_view>

namespace dbslog
{
std::string hexdump(const string_view& array,
                    std::size_t group = 8,
                    std::size_t width = 16);
}
