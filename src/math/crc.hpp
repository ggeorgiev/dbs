//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include <boost/crc.hpp>

namespace math
{
typedef boost::crc_optimal<64, 0x04C11DB7, 0, 0, false, false> CrcProcessor;
typedef uint64_t Crcsum;

inline Crcsum obfuscate(const Crcsum x)
{
    const std::uint8_t* bytes = reinterpret_cast<const std::uint8_t*>(&x);

    Crcsum obfuscated = bytes[0];
    for (size_t i = 1; i < sizeof(x); ++i)
        obfuscated = obfuscated * 101 + bytes[i];

    obfuscated = obfuscated * 101 + bytes[0] ^ bytes[2] ^ bytes[4] ^ bytes[6];
    obfuscated = obfuscated * 101 + bytes[1] ^ bytes[3] ^ bytes[5] ^ bytes[7];

    return obfuscated;
}
}
