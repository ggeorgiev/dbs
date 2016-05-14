//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_header_crc_task.h"
#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/sys/ensure_directory_task.h"
#include "task/sys/execute_command_task.h"
#include "task/sys/parse_stdout_task.h"
#include "im/initialization_manager.hpp"
#include <memory>
#include <mutex>
#include <unordered_set>

namespace task
{
template <typename T, typename H = typename T::Hasher>
class ManagerMixin
{
public:
    typedef T Task;
    typedef std::shared_ptr<Task> TaskSPtr;

    typedef H Hasher;

    TaskSPtr valid(const TaskSPtr& task)
    {
        if (task == nullptr)
            return task;

        std::unique_lock<std::mutex> lock(mTaskMutex);
        return *mTasks.insert(task).first;
    }

    TaskSPtr unique(const TaskSPtr& task)
    {
        if (task == nullptr)
            return task;

        std::unique_lock<std::mutex> lock(mTaskMutex);
        return *mTasks.insert(task).first;
    }

protected:
    std::mutex mTaskMutex;
    unordered_set<TaskSPtr, Hasher, Hasher> mTasks;
};

class Manager : public ManagerMixin<CxxFileCrcTask>,
                public ManagerMixin<CxxHeaderCrcTask>,
                public ManagerMixin<CxxObjectFileCrcTask>,
                public ManagerMixin<CxxProgramCrcTask>,
                public ManagerMixin<EnsureDirectoryTask>,
                public ManagerMixin<ExecuteCommandTask>,
                public ManagerMixin<ParseStdoutTask>

{
public:
    static constexpr int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    using ManagerMixin<CxxFileCrcTask>::valid;
    using ManagerMixin<CxxHeaderCrcTask>::valid;
    using ManagerMixin<CxxObjectFileCrcTask>::valid;
    using ManagerMixin<CxxProgramCrcTask>::valid;
    using ManagerMixin<EnsureDirectoryTask>::valid;
    using ManagerMixin<ExecuteCommandTask>::valid;
    using ManagerMixin<ParseStdoutTask>::valid;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
