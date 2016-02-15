//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "doim/cxx/cxx_program.hpp"
#include "err/err.h"
#include "math/crc.hpp"
#include <memory>
#include <stddef.h>

namespace task
{
class CxxProgramCrcTask;
typedef std::shared_ptr<CxxProgramCrcTask> CxxProgramCrcTaskSPtr;

class CxxProgramCrcTask : public tpool::Task
{
public:
    CxxProgramCrcTask(const doim::CxxProgramSPtr& cxxProgram);

    math::Crcsum crc()
    {
        return mCrcsum;
    }

    ECode operator()() override;
    std::string description() const override;

    struct Hasher
    {
        std::size_t operator()(const CxxProgramCrcTaskSPtr& task) const;

        bool operator()(const CxxProgramCrcTaskSPtr& task1,
                        const CxxProgramCrcTaskSPtr& task2) const;
    };

private:
    math::Crcsum mCrcsum;
    doim::CxxProgramSPtr mCxxProgram;
};

inline std::size_t CxxProgramCrcTask::Hasher::operator()(
    const CxxProgramCrcTaskSPtr& task) const
{
    return std::hash<doim::CxxProgramSPtr>()(task->mCxxProgram);
}

inline bool CxxProgramCrcTask::Hasher::operator()(
    const CxxProgramCrcTaskSPtr& task1, const CxxProgramCrcTaskSPtr& task2) const
{
    return task1->mCxxProgram == task2->mCxxProgram;
}
}
