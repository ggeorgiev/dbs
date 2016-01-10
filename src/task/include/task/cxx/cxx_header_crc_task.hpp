//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_crc_task_mixin.hpp"
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
class CxxHeaderCrcTask : public CxxCrcTaskMixin
{
public:
    CxxHeaderCrcTask(const doim::CxxHeaderSPtr& cxxHeader,
                     doim::CxxHeaderSetSPtr cxxHeaders)
        : mCxxHeader(cxxHeader)
        , mCxxHeaders(cxxHeaders)
    {
    }

    ECode operator()()
    {
        doim::CxxHeaderSet includes;
        EHTest(calculate(mCxxHeader->file(),
                         mCxxHeader->cxxIncludeDirectories(),
                         mCxxHeaders,
                         includes));

        for (const auto& header : includes)
        {
            if (header->type() == doim::CxxHeader::Type::kSystem)
                continue;

            auto task = std::make_shared<CxxHeaderCrcTask>(header, mCxxHeaders);
            EHTest((*task)());
            mCrc64 = mCrc64 ^ task->crc();
        }
        EHEnd;
    }

private:
    doim::CxxHeaderSPtr mCxxHeader;
    doim::CxxHeaderSetSPtr mCxxHeaders;
};
}
