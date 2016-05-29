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
#include "math/crc.hpp"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <shared_ptr>
#include <str>
#include <string_view>
#include <vector>

namespace task
{
class CxxCrcTaskMixin
{
protected:
    template <typename Task>
    ECode calculate(const doim::FsFileSPtr& file,
                    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory,
                    const doim::CxxIncludeDirectorySetSPtr& includeDirectories,
                    math::Crcsum& crcsum)
    {
        const auto& path = file->path();
        if (!boost::filesystem::exists(path))
        {
            crcsum = 0;
            EHEnd;
        }

        std::ifstream fstream(file->path().c_str());
        string content((std::istreambuf_iterator<char>(fstream)),
                       std::istreambuf_iterator<char>());

        parser::CxxParser parser;

        std::vector<doim::CxxIncludeDirectory::CxxHeaderInfo> headersInfo;
        for (const auto& include : parser.includes(content))
        {
            auto currentDirectory =
                include.mType == parser::CxxParser::EIncludeType::kProgrammer
                    ? currentIncludeDirectory
                    : nullptr;
            doim::CxxIncludeDirectory::CxxHeaderInfo headerInfo;
            EHTest(doim::CxxIncludeDirectory::findHeader(string_view(include.mPath),
                                                         currentDirectory,
                                                         includeDirectories,
                                                         headerInfo),
                   file->path());
            headersInfo.push_back(headerInfo);
        }

        std::vector<tpool::TaskSPtr> tasks;
        tasks.reserve(headersInfo.size());

        for (const auto& headerInfo : headersInfo)
        {
            if (headerInfo.mHeader->type() == doim::CxxHeader::EType::kSystem)
                continue;

            auto task = Task::valid(headerInfo.mHeader, headerInfo.mIncludeDirectory);
            task::gTPool->ensureScheduled(task);
            tasks.push_back(task);
        }

        auto group = tpool::TaskGroup::make(task::gTPool, 0, tasks);
        task::gTPool->ensureScheduled(group);

        math::CrcProcessor crcProcessor;
        crcProcessor.process_bytes(content.data(), content.size());

        EHTest(group->join());

        math::Crcsum x = 0;
        for (const auto& task : tasks)
        {
            auto n = std::static_pointer_cast<Task>(task)->crc();
            if (n == 0)
            {
                crcsum = 0;
                EHEnd;
            }
            x ^= n;
        }

        crcProcessor.process_bytes(&x, sizeof(x));
        crcsum = crcProcessor.checksum();

        EHEnd;
    }
};
}
