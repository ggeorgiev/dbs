//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/tpool.h"
#include "tpool/task.h"
#include "tpool/task_group.h"
#include "parser/cxx/cxx_parser.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "err/err_cppformat.h"
#include <boost/filesystem/operations.hpp>
#include "math/crc.hpp"
#include <algorithm>
#include <experimental/string_view>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

namespace task
{
class CxxCrcTaskMixin
{
public:
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
        const auto& path = file->path(nullptr);
        if (!boost::filesystem::exists(path))
        {
            mCrcsum = 0;
            EHEnd;
        }

        std::ifstream fstream(file->path(nullptr).c_str());
        std::string content((std::istreambuf_iterator<char>(fstream)),
                            std::istreambuf_iterator<char>());

        parser::CxxParser parser;

        std::vector<HeaderDirectory> headerDirectories;
        for (const auto& include : parser.includes(content))
        {
            HeaderDirectory headerDirectory;
            EHTest(findInclude(std::experimental::string_view(include.mPath),
                               include.mType ==
                                       parser::CxxParser::EIncludeType::kProgrammerDefined
                                   ? currentIncludeDirectory
                                   : nullptr,
                               includeDirectories,
                               headerDirectory),
                   file->path());
            headerDirectories.push_back(headerDirectory);
        }

        std::vector<tpool::TaskSPtr> tasks;
        tasks.reserve(headerDirectories.size());

        for (const auto& headerDirectory : headerDirectories)
        {
            if (headerDirectory.first->type() == doim::CxxHeader::Type::kSystem)
                continue;

            auto task = Task::valid(
                std::make_shared<Task>(headerDirectory.first, headerDirectory.second));
            task::gTPool->ensureScheduled(task);
            tasks.push_back(task);
        }

        auto group = std::make_shared<tpool::TaskGroup>(task::gTPool, 0, tasks);
        task::gTPool->ensureScheduled(group);

        math::CrcProcessor crcProcessor;
        crcProcessor.process_bytes(content.data(), content.size());

        EHTest(group->join());

        std::vector<math::Crcsum> crcs;
        crcs.reserve(headerDirectories.size());
        for (const auto& task : tasks)
            crcs.push_back(std::static_pointer_cast<Task>(task)->crc());

        std::sort(crcs.begin(), crcs.end());

        if (crcs.size() > 0 && crcs[0] == 0)
        {
            mCrcsum = 0;
            EHEnd;
        }

        crcProcessor.process_bytes(crcs.data(), sizeof(math::Crcsum) * crcs.size());
        mCrcsum = crcProcessor.checksum();

        EHEnd;
    }

    math::Crcsum mCrcsum;
};
}
