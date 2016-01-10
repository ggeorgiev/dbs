//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_crc_task_mixin.hpp"
#include "task/cxx/cxx_header_crc_task.hpp"
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
class CxxFileCrcTask : public CxxCrcTaskMixin
{
public:
    CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile)
        : mCxxFile(cxxFile)
    {
    }

    ECode operator()()
    {
        doim::CxxHeaderSet includes;
        EHTest(calculate(mCxxFile->file(),
                         mCxxFile->cxxIncludeDirectories(),
                         mCxxFile->cxxHeaders(),
                         includes));

        for (const auto& header : includes)
        {
            if (header->type() == doim::CxxHeader::Type::kSystem)
                continue;

            auto task =
                std::make_shared<CxxHeaderCrcTask>(header, mCxxFile->cxxHeaders());
            EHTest((*task)());
            mCrc64 = mCrc64 ^ task->crc();
        }

        EHEnd;
    }

private:
    doim::CxxFileSPtr mCxxFile;
};
}
