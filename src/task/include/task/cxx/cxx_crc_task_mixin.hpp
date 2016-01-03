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
class CxxCrcTaskMixin : public tpool::Task
{
public:
    typedef boost::crc_optimal<64, 0x04C11DB7, 0, 0, false, false> Crc;

    CxxCrcTaskMixin()
        : tpool::Task(0, nullptr)
    {
    }

    ECode findInclude(const std::experimental::string_view& include,
                      const doim::CxxIncludeDirectorySetSPtr& includeDirectories,
                      const doim::CxxHeaderSetSPtr& headers,
                      doim::CxxHeaderSPtr& cxxHeader)
    {
        doim::FsFileSPtr file;
        for (const auto& directory : *includeDirectories)
        {
            const auto& curr = doim::gManager->find(
                doim::gManager->createFile(directory->directory(), include));
            if (curr == nullptr)
                continue;

            if (directory->headerFiles()->count(curr) == 0)
                continue;

            if (file != nullptr)
                EHBan(kTooMany, include);

            file = curr;
        }

        if (file == nullptr)
            EHBan(kNotFound, include);

        for (const auto& header : *headers)
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

    uint64_t crc()
    {
        return mCrc64;
    }

    ECode calculate(const doim::FsFileSPtr& file,
                    const doim::CxxIncludeDirectorySetSPtr& includeDirectories,
                    const doim::CxxHeaderSetSPtr& headers,
                    doim::CxxHeaderSet& includes)
    {
        std::ifstream fstream(file->path(nullptr));
        std::string content((std::istreambuf_iterator<char>(fstream)),
                            std::istreambuf_iterator<char>());

        Crc crc;
        crc.process_bytes(content.data(), content.size());

        mCrc64 = crc.checksum();

        parser::CxxParser parser;

        includes.clear();
        for (const auto& include : parser.includes(content))
        {
            doim::CxxHeaderSPtr header;
            EHTest(findInclude(std::experimental::string_view(include.data() + 1,
                                                              include.size() - 2),
                               includeDirectories,
                               headers,
                               header));
            includes.insert(header);
        }
        EHEnd;
    }

protected:
    uint64_t mCrc64;
};
}
