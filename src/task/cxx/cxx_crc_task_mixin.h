//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "parser/cxx/cxx_parser.hpp"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "err/err_cppformat.h"
#include "math/crc.hpp"
#include <algorithm>
#include <experimental/string_view>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace task
{
class CxxCrcTaskMixin : public tpool::Task
{
public:
    CxxCrcTaskMixin()
        : tpool::Task(0)
    {
    }

    math::Crcsum crc()
    {
        return mCrcsum;
    }

protected:
    typedef std::pair<doim::CxxHeaderSPtr, doim::CxxIncludeDirectorySPtr> HeaderDirectory;

    doim::CxxHeaderSPtr findInclude(
        const std::experimental::string_view& include,
        const doim::CxxIncludeDirectorySPtr& includeDirectory);

    ECode findInclude(const std::experimental::string_view& include,
                      const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory,
                      const doim::CxxIncludeDirectorySetSPtr& includeDirectories,
                      HeaderDirectory& headerDirectory);

    template <typename Task>
    ECode calculate(const doim::FsFileSPtr& file,
                    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory,
                    const doim::CxxIncludeDirectorySetSPtr& includeDirectories)
    {
        math::CrcProcessor crcProcessor;

        std::ifstream fstream(file->path(nullptr).c_str());
        std::string content((std::istreambuf_iterator<char>(fstream)),
                            std::istreambuf_iterator<char>());

        crcProcessor.process_bytes(content.data(), content.size());

        parser::CxxParser parser;

        std::vector<HeaderDirectory> headerDirectories;
        for (const auto& include : parser.includes(content))
        {
            HeaderDirectory headerDirectory;
            EHTest(findInclude(std::experimental::string_view(include.data() + 1,
                                                              include.size() - 2),
                               currentIncludeDirectory,
                               includeDirectories,
                               headerDirectory),
                   file->path());
            headerDirectories.push_back(headerDirectory);
        }

        std::vector<math::Crcsum> crcs;
        crcs.reserve(headerDirectories.size());

        for (const auto& headerDirectory : headerDirectories)
        {
            if (headerDirectory.first->type() == doim::CxxHeader::Type::kSystem)
                continue;

            auto task = Task::valid(
                std::make_shared<Task>(headerDirectory.first, headerDirectory.second));
            EHTest(task->join());
            crcs.push_back(task->crc());
        }

        std::sort(crcs.begin(), crcs.end());

        crcProcessor.process_bytes(crcs.data(), sizeof(math::Crcsum) * crcs.size());
        mCrcsum = crcProcessor.checksum();

        EHEnd;
    }

    math::Crcsum mCrcsum;
};
}
