//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_crc_task_mixin.hpp"
#include "doim/cxx/cxx_file.h"
#include "err/err.h"
#include <iosfwd>
#include <memory>
#include <stddef.h>

namespace task
{
class CxxFileCrcTask;

typedef std::shared_ptr<CxxFileCrcTask> CxxFileCrcTaskSPtr;

class CxxFileCrcTask : public CxxCrcTaskMixin
{
public:
    CxxFileCrcTask(const doim::CxxFileSPtr& cxxFile);

    ECode operator()() override;
    std::string description() const override;

    struct Hasher
    {
        std::size_t operator()(const CxxFileCrcTaskSPtr& task) const;
        bool operator()(const CxxFileCrcTaskSPtr& task1,
                        const CxxFileCrcTaskSPtr& task2) const;
    };

private:
    doim::CxxFileSPtr mCxxFile;
};

inline std::size_t CxxFileCrcTask::Hasher::operator()(
    const CxxFileCrcTaskSPtr& task) const
{
    return std::hash<doim::CxxFileSPtr>()(task->mCxxFile);
}

inline bool CxxFileCrcTask::Hasher::operator()(const CxxFileCrcTaskSPtr& task1,
                                               const CxxFileCrcTaskSPtr& task2) const
{
    return task1->mCxxFile == task2->mCxxFile;
}
}
