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
                     const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory)
        : mCxxHeader(cxxHeader)
        , mCurrentIncludeDirectory(currentIncludeDirectory)
    {
    }

    ECode operator()() override;

    struct Hasher
    {
        std::size_t operator()(const CxxHeaderCrcTaskSPtr& task) const
        {
            auto seed = hashCH(task->mCxxHeader);
            boost::hash_combine(seed, hashCID(task->mCurrentIncludeDirectory));
            return seed;
        }

        bool operator()(const CxxHeaderCrcTaskSPtr& task1,
                        const CxxHeaderCrcTaskSPtr& task2) const
        {
            return task1->mCxxHeader == task2->mCxxHeader &&
                   task1->mCurrentIncludeDirectory == task2->mCurrentIncludeDirectory;
        }

        std::hash<doim::CxxHeaderSPtr> hashCH;
        std::hash<doim::CxxIncludeDirectorySPtr> hashCID;
    };

private:
    doim::CxxHeaderSPtr mCxxHeader;
    doim::CxxIncludeDirectorySPtr mCurrentIncludeDirectory;
};
}
