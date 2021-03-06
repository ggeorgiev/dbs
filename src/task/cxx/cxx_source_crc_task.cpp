//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_source_crc_task.h"
#include "task/cxx/cxx_source_headers_task.h"
#include "task/protobuf/protobuf_file_crc_task.h"
#include "task/tpool.h"
#include "tpool/task_group.h"
#include "logex/log.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/protobuf/protobuf_file.h"
#include "doim/element.hpp"
#include "err/err.h"
#include "err/err_assert.h"
#include "err/raii.hpp"
#include "math/crc.hpp"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>
#include <unordered>
#include <utility>
#include <variant>
#include <vector>

namespace task
{
bool CxxSourceCrcTask::check() const
{
    return apply_visitor(doim::vst::isUnique, cxxSource()) &&
           (currentIncludeDirectory() == nullptr ||
            currentIncludeDirectory()->isUnique());
}

static doim::TagSetSPtr gTags =
    doim::TagSet::global({&doim::gCrcTag, &doim::gInspectTag, &doim::gCxxTag}, gTags);

doim::TagSetSPtr CxxSourceCrcTask::tags() const
{
    return gTags;
}

ECode CxxSourceCrcTask::operator()()
{
    const auto& path = apply_visitor(doim::vst::path(), cxxSource());

    defer(LOGEX(tags(),
                "{} for {} is {:x}",
                depth() == EDepth::kOne ? "Crc" : "Deep Crc",
                path,
                mCrcsum));

    auto origin =
        apply_visitor([](auto const& element) { return element->origin(); }, cxxSource());

    if (!apply_visitor(vst::isNullptr, origin))
    {
        if (origin.type() == typeid(doim::ProtobufFileSPtr))
        {
            auto originTask =
                ProtobufFileCrcTask::valid(boost::get<doim::ProtobufFileSPtr>(origin));
            gTPool->ensureScheduled(originTask);
            EHTest(originTask->join());
            mCrcsum = originTask->crc();
        }
        else
            ASSERT(false);
        EHEnd;
    }

    if (depth() == EDepth::kOne)
    {
        EHTest(one());
        EHEnd;
    }

    auto crcTask = valid(EDepth::kOne, cxxSource(), currentIncludeDirectory());
    gTPool->ensureScheduled(crcTask);

    auto headersTask = CxxSourceHeadersTask::valid(CxxSourceHeadersTask::EDepth::kAll,
                                                   cxxSource(),
                                                   currentIncludeDirectory());
    gTPool->ensureScheduled(headersTask);
    EHTest(headersTask->join());

    const auto& headersInfo = headersTask->headersInfo();
    std::vector<CxxSourceCrcTaskSPtr> tasks;
    tasks.reserve(headersInfo.size() + 1);

    tasks.push_back(crcTask);

    for (const auto& headerInfo : headersInfo)
    {
        if (headerInfo.mHeader->type() == doim::CxxHeader::EType::kSystem)
            continue;

        auto task = CxxSourceCrcTask::valid(CxxSourceCrcTask::EDepth::kOne,
                                            headerInfo.mHeader,
                                            headerInfo.mIncludeDirectory);
        task::gTPool->ensureScheduled(task);
        tasks.push_back(task);
    }

    auto group = tpool::TaskGroup::make(task::gTPool, 0, tasks);
    task::gTPool->ensureScheduled(group);

    EHTest(group->join());

    unordered_set<math::Crcsum> crcs;

    math::Crcsum x = 0;
    for (const auto& task : tasks)
    {
        auto n = task->crc();
        if (n == 0)
        {
            mCrcsum = 0;
            EHEnd;
        }
        auto unique = crcs.insert(n).second;
        if (!unique)
            EHBan(kTooMany, "There are at least two items with the same crc");

        x ^= n;
    }

    mCrcsum = math::obfuscate(x);
    EHEnd;
}

ECode CxxSourceCrcTask::one()
{
    const auto& path = apply_visitor(doim::vst::path(), cxxSource());

    if (!boost::filesystem::exists(path))
    {
        mCrcsum = 0;
        EHEnd;
    }

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(content.data(), content.size());
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

string CxxSourceCrcTask::description() const
{
    auto path = apply_visitor(doim::vst::path(), cxxSource());

    return "Cxx source file crc " + path;
}

} // namespace task
