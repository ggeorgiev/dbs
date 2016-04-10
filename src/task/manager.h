//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_file_crc_task.h"        // IWYU pragma: keep
#include "task/cxx/cxx_header_crc_task.h"      // IWYU pragma: keep
#include "task/cxx/cxx_object_file_crc_task.h" // IWYU pragma: keep
#include "task/cxx/cxx_program_crc_task.h"     // IWYU pragma: keep
#include "task/db/db_put_task.h"               // IWYU pragma: keep
#include "task/sys/ensure_directory_task.h"    // IWYU pragma: keep
#include "task/sys/execute_command_task.h"     // IWYU pragma: keep
#include "im/initialization_manager.hpp"
#include <memory>
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
        return *mTasks.insert(task).first;
    }

protected:
    std::unordered_set<TaskSPtr, Hasher, Hasher> mTasks;
};

class Manager : public ManagerMixin<DbPutTask>,
                public ManagerMixin<CxxFileCrcTask>,
                public ManagerMixin<CxxHeaderCrcTask>,
                public ManagerMixin<CxxObjectFileCrcTask>,
                public ManagerMixin<CxxProgramCrcTask>,
                public ManagerMixin<EnsureDirectoryTask>,
                public ManagerMixin<ExecuteCommandTask>

{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    using ManagerMixin<DbPutTask>::valid;
    using ManagerMixin<CxxFileCrcTask>::valid;
    using ManagerMixin<CxxHeaderCrcTask>::valid;
    using ManagerMixin<CxxObjectFileCrcTask>::valid;
    using ManagerMixin<CxxProgramCrcTask>::valid;
    using ManagerMixin<EnsureDirectoryTask>::valid;
    using ManagerMixin<ExecuteCommandTask>::valid;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
