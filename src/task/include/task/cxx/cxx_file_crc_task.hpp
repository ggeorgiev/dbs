//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.hpp"

#include "tpool/task.hpp"

#include <boost/crc.hpp>

#include <fstream>

namespace task
{
class CxxFileCrcTask : public tpool::Task
{
public:
    typedef boost::crc_optimal<64, 0x04C11DB7, 0, 0, false, false> Crc;

    CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile)
        : tpool::Task(0, nullptr)
        , mCxxFile(cxxFile)
    {
    }

    ECode operator()()
    {
        std::ifstream fstream(mCxxFile->file()->path(nullptr));
        std::string content((std::istreambuf_iterator<char>(fstream)),
                            std::istreambuf_iterator<char>());

        Crc crc;
        crc.process_bytes(content.data(), content.size());

        mCrc64 = crc.checksum();
        EHEnd;
    }

    uint64_t crc()
    {
        return mCrc64;
    }

private:
    doim::CxxFileSPtr mCxxFile;
    uint64_t mCrc64;
};
}
