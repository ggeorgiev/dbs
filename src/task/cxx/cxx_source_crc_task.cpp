//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_source_crc_task.h"
#include "task/cxx/cxx_source_headers_task.h"
#include "task/protobuf/protobuf_file_crc_task.h"
#include "task/tpool.h"
#include "tpool/task_group.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/protobuf/protobuf_file.h"
#include "doim/element.hpp"
#include "err/err.h"
#include "err/err_assert.h"
#include "err/raii.hpp"
#include "log/log.h"
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
CxxSourceCrcTask::CxxSourceCrcTask(
    const CxxSourceSPtr& cxxSource,
    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory)
    : CrcTask(cxxSource, currentIncludeDirectory)
{
    ASSERT(apply_visitor(doim::vst::isUnique, cxxSource));
    ASSERT(currentIncludeDirectory->isUnique());
}

ECode CxxSourceCrcTask::operator()()
{
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

    const auto& path = apply_visitor(doim::vst::path, cxxSource());

    Defer defer([=] { DLOG("Crc for {0} is {1:x}", path, mCrcsum); });

    if (!boost::filesystem::exists(path))
    {
        mCrcsum = 0;
        EHEnd;
    }

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    auto headersTask =
        CxxSourceHeadersTask::valid(cxxSource(), currentIncludeDirectory());
    gTPool->ensureScheduled(headersTask);
    EHTest(headersTask->join());

    const auto& headersInfo = headersTask->headersInfo();
    std::vector<CxxSourceCrcTaskSPtr> tasks;
    tasks.reserve(headersInfo.size());

    for (const auto& headerInfo : headersInfo)
    {
        if (headerInfo.mHeader->type() == doim::CxxHeader::EType::kSystem)
            continue;

        auto task =
            CxxSourceCrcTask::valid(headerInfo.mHeader, headerInfo.mIncludeDirectory);
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
            mCrcsum = 0;
            EHEnd;
        }
        auto unique = crcs.insert(n).second;
        if (!unique)
            EHBan(kTooMany, "There are at least two items with the same crc");

        x ^= n;
    }

    math::Crcsum sum = 0;
    for (size_t i = 0; i < sizeof(x); ++i)
        sum = sum * 101 + reinterpret_cast<std::uint8_t*>(&x)[i];

    crcProcessor.process_bytes(&sum, sizeof(sum));
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

string CxxSourceCrcTask::description() const
{
    auto path = apply_visitor(doim::vst::path, cxxSource());

    return "Cxx source file crc " + path;
}

} // namespace task
