//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "parser/cxx/cxx_parser.hpp"
#include "doim/cxx/cxx_file.hpp"
#include "doim/manager.h"
#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <fstream>
#include <regex>

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

    ECode findInclude(const std::experimental::string_view& include,
                      doim::CxxHeaderSPtr& cxxHeader)
    {
        doim::FsFileSPtr file;
        for (const auto& directory : *mCxxFile->cxxIncludeDirectories())
        {
            const auto& curr = doim::gManager->find(
                doim::gManager->createFile(directory->directory(), include));
            if (curr == nullptr)
                continue;

            if (file != nullptr)
                EHBan(kTooMany, include);

            file = curr;
        }

        if (file == nullptr)
            EHBan(kNotFound, include);

        for (const auto& header : *mCxxFile->cxxHeaders())
        {
            if (file == header->file())
            {
                cxxHeader = header;
                EHEnd;
            }
        }

        EHBan(kNotFound, include);
        EHEnd;
    }

    ECode operator()()
    {
        std::ifstream fstream(mCxxFile->file()->path(nullptr));
        std::string content((std::istreambuf_iterator<char>(fstream)),
                            std::istreambuf_iterator<char>());

        Crc crc;
        crc.process_bytes(content.data(), content.size());

        mCrc64 = crc.checksum();

        parser::CxxParser parser;
        const auto& includes = parser.includes(content);

        for (const auto& include : includes)
        {
            doim::CxxHeaderSPtr file;
            EHTest(findInclude(std::experimental::string_view(include.data() + 1,
                                                              include.size() - 2),
                               file));
        }
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
