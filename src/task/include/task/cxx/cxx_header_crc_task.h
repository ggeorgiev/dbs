//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_crc_task_mixin.hpp"
#include "doim/cxx/cxx_header.hpp"
#include "err/err.h"
#include <memory>
#include <stddef.h>

namespace task
{
class CxxHeaderCrcTask;

typedef std::shared_ptr<CxxHeaderCrcTask> CxxHeaderCrcTaskSPtr;

class CxxHeaderCrcTask : public CxxCrcTaskMixin
{
public:
    CxxHeaderCrcTask(const doim::CxxHeaderSPtr& cxxHeader,
                     doim::CxxHeaderSetSPtr cxxHeaders)
        : mCxxHeader(cxxHeader)
        , mCxxHeaders(cxxHeaders)
    {
    }

    ECode operator()() override;

    struct Hasher
    {
        std::size_t operator()(const CxxHeaderCrcTaskSPtr& task) const
        {
            return std::hash<doim::CxxHeaderSPtr>()(task->mCxxHeader) ^
                   std::hash<doim::CxxHeaderSetSPtr>()(task->mCxxHeaders);
        }

        bool operator()(const CxxHeaderCrcTaskSPtr& task1,
                        const CxxHeaderCrcTaskSPtr& task2) const
        {
            return task1->mCxxHeader == task2->mCxxHeader &&
                   task1->mCxxHeaders == task2->mCxxHeaders;
        }
    };

private:
    doim::CxxHeaderSPtr mCxxHeader;
    doim::CxxHeaderSetSPtr mCxxHeaders;
};
}
