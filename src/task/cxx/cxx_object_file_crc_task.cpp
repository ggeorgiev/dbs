//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_source_crc_task.h"
#include "task/tpool.h"
#include "logex/log.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "err/err_assert.h"
#include "err/raii.hpp"
#include "math/crc.hpp"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>

namespace task
{
bool CxxObjectFileCrcTask::check() const
{
    return cxxObjectFile() != nullptr && cxxObjectFile()->isUnique();
}

static doim::TagSetSPtr gTags = doim::TagSet::global(
    {&doim::gCrcTag, &doim::gInspectTag, &doim::gCxxTag, &doim::gObjTag}, gTags);

doim::TagSetSPtr CxxObjectFileCrcTask::tags() const
{
    return gTags;
}

ECode CxxObjectFileCrcTask::operator()()
{
    const auto& path = cxxObjectFile()->file()->path();

    defer(LOGEX(tags(), "Crc for {} is {:x}", path, mCrcsum));

    if (!boost::filesystem::exists(path))
    {
        mCrcsum = 0;
        EHEnd;
    }

    auto task = CxxSourceCrcTask::valid(CxxSourceCrcTask::EDepth::kAll,
                                        cxxObjectFile()->cxxFile(),
                                        nullptr);
    task::gTPool->ensureScheduled(task);

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    math::CrcProcessor crcProcessor;
    crcProcessor.process_bytes(content.data(), content.size());

    EHTest(task->join());
    math::Crcsum crc = task->crc();
    if (crc == 0)
    {
        mCrcsum = 0;
        EHEnd;
    }
    crcProcessor.process_bytes(&crc, sizeof(crc));
    mCrcsum = crcProcessor.checksum();

    EHEnd;
}

string CxxObjectFileCrcTask::description() const
{
    return "Cxx object file crc " + cxxObjectFile()->file()->path();
}

} // namespace task
