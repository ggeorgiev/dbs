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
    static CxxHeaderCrcTaskSPtr valid(const CxxHeaderCrcTaskSPtr& task);

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
            // Task mCxxHeaders are there just for reference and even in
            // different contexts the list might be bigger or smaller the end result
            // should always be the same - this is why it does not need to be in the
            // identification of the task.

            // TODO: we might add this back if we fix the need of providing different
            //       sets. Not so easy though - because of circularity.
            return std::hash<doim::CxxHeaderSPtr>()(task->mCxxHeader);
        }

        bool operator()(const CxxHeaderCrcTaskSPtr& task1,
                        const CxxHeaderCrcTaskSPtr& task2) const
        {
            return task1->mCxxHeader == task2->mCxxHeader;
        }
    };

private:
    doim::CxxHeaderSPtr mCxxHeader;
    doim::CxxHeaderSetSPtr mCxxHeaders;
};
}
