//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_source_headers_task.h"
#include "task/tpool.h"
#include "tpool/task_group.h"
#include "parser/cxx/cxx_parser.h"
#include "doim/cxx/cxx_header.h" // IWYU pragma: keep
#include "doim/fs/fs_file.h"
#include "doim/element.hpp"
#include "err/err_assert.h"
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>
#include <string_view>
#include <unordered>
#include <variant>

namespace task
{
CxxSourceHeadersTask::CxxSourceHeadersTask(
    const EDepth depth,
    const CxxSourceSPtr& cxxSource,
    const doim::CxxIncludeDirectorySPtr& cxxIncludeDirectory)
    : Element(depth, cxxSource, cxxIncludeDirectory)
{
    ASSERT(apply_visitor(doim::vst::isUnique, cxxSource));
}

ECode CxxSourceHeadersTask::operator()()
{
    if (depth() == EDepth::kOne)
    {
        EHTest(one());
        EHEnd;
    }

    unordered_set<doim::CxxHeaderSPtr> headers;

    auto headersTask = valid(EDepth::kOne, cxxSource(), cxxIncludeDirectory());
    gTPool->ensureScheduled(headersTask);
    EHTest(headersTask->join());

    std::vector<CxxSourceHeadersTaskSPtr> tasks;
    tasks.push_back(headersTask);

    // Recursive algorithm will be a problem getting in endless loops in case of circular
    // dependency. Instead the algorithm loops over the currently known headers and
    // appends obtains their direct dependencies. After a certain iterations all headers
    // that depends on directly and indirectly will accumulated and tracked.
    for (;;)
    {
        std::vector<CxxSourceHeadersTaskSPtr> newTasks;
        for (auto task : tasks)
        {
            const auto& headersInfo = task->headersInfo();
            for (const auto& headerInfo : headersInfo)
            {
                if (headers.find(headerInfo.mHeader) != headers.end())
                {
                    DLOG("Is already in {}", headerInfo.mHeader->file()->path());
                    continue;
                }

                mHeadersInfo.push_back(headerInfo);
                headers.insert(headerInfo.mHeader);
                DLOG("Insert {}", headerInfo.mHeader->file()->path());

                if (headerInfo.mHeader->type() == doim::CxxHeader::EType::kSystem)
                    continue;

                auto task = CxxSourceHeadersTask::valid(EDepth::kOne,
                                                        headerInfo.mHeader,
                                                        headerInfo.mIncludeDirectory);
                task::gTPool->ensureScheduled(task);
                newTasks.push_back(task);
            }
        }

        if (newTasks.empty())
            break;

        tasks.swap(newTasks);

        auto group = tpool::TaskGroup::make(task::gTPool, 0, tasks);
        task::gTPool->ensureScheduled(group);
        EHTest(group->join());
    }

    EHEnd;
}

ECode CxxSourceHeadersTask::one()
{
    const auto& path = apply_visitor(doim::vst::path, cxxSource());

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    const auto& includeDirectories =
        apply_visitor(doim::vst::cxxIncludeDirectories, cxxSource());

    parser::CxxParser parser;

    for (const auto& include : parser.includes(content))
    {
        auto currentDirectory =
            include.mType == parser::CxxParser::EIncludeType::kProgrammer
                ? cxxIncludeDirectory()
                : nullptr;
        doim::CxxIncludeDirectory::CxxHeaderInfo headerInfo;
        EHTest(doim::CxxIncludeDirectory::findHeader(string_view(include.mPath),
                                                     currentDirectory,
                                                     includeDirectories,
                                                     headerInfo),
               path);
        mHeadersInfo.push_back(headerInfo);
    }
    EHEnd;
}

string CxxSourceHeadersTask::description() const
{
    auto path = apply_visitor(doim::vst::path, cxxSource());
    return "Cxx file headers " + path;
}

} // namespace task
