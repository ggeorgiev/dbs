//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include <sstream>
#include <string>
#include <string_view>

namespace dbslog
{
inline std::string dump(const string_view& array, std::size_t width = 16)
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
        for (std::size_t pass = 1; pass <= 2; ++pass)
        {
            for (const char* next = line; next != end && next != line + width; ++next)
            {
                char ch = *next;
                switch (pass)
                {
                    case 1:
                        stream << (ch < 32 ? '.' : ch);
                        break;
                    case 2:
                        if (next != line)
                            stream << " ";
                        stream.width(2);
                        stream.fill('0');
                        stream << std::hex << std::uppercase
                               << static_cast<int>(static_cast<unsigned char>(ch));
                        break;
                }
            }
            if (pass == 1 && lineLength != width)
                stream << std::string(width - lineLength, ' ');
            stream << " ";
        }
        stream << std::endl;
        line = line + lineLength;
    }

    return stream.str();
}
}
