//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <boost/crc.hpp>

namespace math
{
typedef boost::crc_optimal<64, 0x04C11DB7, 0, 0, false, false> CrcProcessor;
typedef uint64_t Crcsum;
}
