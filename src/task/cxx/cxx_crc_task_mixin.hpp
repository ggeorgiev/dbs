//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_source_headers_task.h"
#include "task/tpool.h"
#include "tpool/task.h"
#include "tpool/task_group.h"
#include "parser/cxx/cxx_parser.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "log/log.h"
#include "math/crc.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/variant/variant.hpp>
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
    ECode calculate(boost::variant<doim::CxxFileSPtr, doim::CxxHeaderSPtr> cxxSourceSPtr,
                    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory,
                    math::Crcsum& crcsum)
    {
        const auto& path = boost::apply_visitor(
            [](auto const& source) { return source->file()->path(); }, cxxSourceSPtr);

        if (!boost::filesystem::exists(path))
        {
            crcsum = 0;
            EHEnd;
        }

        std::ifstream fstream(path.c_str());
        string content((std::istreambuf_iterator<char>(fstream)),
                       std::istreambuf_iterator<char>());

        auto headersTask =
            CxxSourceHeadersTask::valid(cxxSourceSPtr, currentIncludeDirectory);
        gTPool->ensureScheduled(headersTask);
        EHTest(headersTask->join());

        const auto& headersInfo = headersTask->headersInfo();
        std::vector<shared_ptr<Task>> tasks;
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

        unordered_set<math::Crcsum> crcs;

        math::Crcsum x = 0;
        for (const auto& task : tasks)
        {
            auto n = task->crc();
            if (n == 0)
            {
                crcsum = 0;
                EHEnd;
            }
            auto unique = crcs.insert(n).second;
            if (!unique)
                EHBan(kTooMany, "There are at least two items with the same crc");
            x ^= n;
        }

        crcProcessor.process_bytes(&x, sizeof(x));
        crcsum = crcProcessor.checksum();
        EHEnd;
    }
};
}
