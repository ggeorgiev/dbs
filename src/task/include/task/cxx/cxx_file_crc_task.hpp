//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.hpp"

#include "parser/cxx/cxx_parser.hpp"

#include "tpool/task.hpp"

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

    ECode find(const doim::CxxIncludeDirectorySetSPtr& directories,
               const std::string& file,
               doim::FsFileSPtr& fsFile)
    {
        for (const auto& directory : *mCxxFile->cxxIncludeDirectories())
        {
            boost::filesystem::path path(directory->directory()->path(nullptr) + file);
            boost::system::error_code ec;
            if (boost::filesystem::is_regular_file(path, ec))
            {
                EHEnd;
            }
        }
        EHBan(kNotFound, file);
    }

    ECode operator()()
    {
        std::ifstream fstream(mCxxFile->file()->path(nullptr));
        std::string content((std::istreambuf_iterator<char>(fstream)),
                            std::istreambuf_iterator<char>());

        Crc crc;
        crc.process_bytes(content.data(), content.size());

        mCrc64 = crc.checksum();
        /*
                parser::CxxParser parser;
                const auto& includes = parser.includes(content);

                for (const auto& include : includes)
                {
                    doim::FsFileSPtr file;
                    EHTest(find(mCxxFile->cxxIncludeDirectories(), include, file));
                }
        */
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
