//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <stddef.h>

namespace dbslog
{
std::string hexdump(const string_view& array,
                    std::size_t group = 8,
                    std::size_t width = 16)
{
    const auto start = array.begin();
    const auto end = array.end();

    std::stringstream stream;

    auto line = start;
    while (line != end)
    {
        stream.width(4);
        stream.fill('0');
        stream << std::hex << line - start << " : ";
        std::size_t lineLength = std::min(width, static_cast<std::size_t>(end - line));
        for (auto next = line; next != end && next != line + width; ++next)
            stream << (*next < 32 ? '.' : *next);

        stream << std::string(width - lineLength, ' ');
        stream << " ";

        std::size_t split = group;
        for (auto next = line; next != end && next != line + width; ++next)
        {
            if (split-- == 0)
            {
                split = group;
                stream << " ";
            }
            if (next != line)
                stream << " ";
            stream.width(2);
            stream.fill('0');
            int byte = static_cast<unsigned int>(static_cast<unsigned char>(*next));
            stream << std::hex << std::uppercase << byte;
            stream << " ";
        }
        stream << std::endl;
        line = line + lineLength;
    }

    return stream.str();
}
}
