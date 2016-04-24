//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "tpool/task_callback.h"
#include "tool/cxx_compiler.h"
#include "db/database.h"

namespace engine
{
CxxEngine::CxxEngine(const tool::CxxCompilerSPtr& compiler)
    : mCompiler(compiler)
{
}

tpool::TaskSPtr CxxEngine::updateDbTask(const tpool::TaskSPtr& task,
                                        const doim::DbKeySPtr& key,
                                        const doim::DbValueSPtr& value) const
{
    auto onFinish = [key, value](const tpool::TaskSPtr&) -> ECode {
        auto updateTask = std::make_shared<task::DbPutTask>(key, value);
        EHTest(updateTask->execute());
        EHEnd;
    };

    return std::make_shared<tpool::TaskCallback>(0, task, onFinish);
}

tpool::TaskSPtr CxxEngine::compileTask(const doim::FsDirectorySPtr& directory,
                                       const doim::CxxObjectFileSPtr& objectFile)
{
    auto crcTask = task::gManager->valid(
        std::make_shared<task::CxxFileCrcTask>(objectFile->cxxFile()));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish = [this, self, directory, objectFile](
        const tpool::TaskSPtr& task) -> ECode {

        auto key = std::make_shared<doim::DbKey>("file:" + objectFile->file()->path());
        key = doim::gManager->unique(key);

        math::Crcsum crc;
        db::gDatabase->get(key->bytes(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxFileCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Cxx object '{}' is already built.",
                 objectFile->file()->path(directory));
            EHEnd;
        }

        auto mkdirTask = task::gManager->valid(
            std::make_shared<task::EnsureDirectoryTask>(objectFile->file()->directory()));

        auto value = std::make_shared<doim::DbValue>(crcTask->crc());

        tpool::TaskCallback::Function onFinish = [key, value](
            const tpool::TaskSPtr&) -> ECode {
            auto updateTask = std::make_shared<task::DbPutTask>(key, value);
            EHTest(updateTask->execute());
            EHEnd;
        };

        auto compileTask = mCompiler->compileCommand(directory, objectFile);

        auto tasks = std::vector<tpool::TaskSPtr>{mkdirTask, compileTask};
        auto seqTask =
            task::gManager->unique(std::make_shared<tpool::TaskSequence>(0, tasks));
        task::gTPool->ensureScheduled(seqTask);

        auto seqCbTask = std::make_shared<tpool::TaskCallback>(0, seqTask, onFinish);
        task::gTPool->ensureScheduled(seqCbTask);
        EHTest(seqCbTask->join());

        EHEnd;
    };

    auto task = std::make_shared<tpool::TaskCallback>(0, crcTask, onFinish);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::buildObjects(const doim::FsDirectorySPtr& directory,
                                        const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    const auto& objectFiles = cxxProgram->cxxObjectFiles();

    std::vector<tpool::TaskSPtr> allTasks;
    for (const auto& objectFile : *objectFiles)
    {
        tpool::TaskSPtr task = compileTask(directory, objectFile);
        allTasks.push_back(task);
    }

    auto group = std::make_shared<tpool::TaskGroup>(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, directory, program, objectFiles](
            const tpool::TaskSPtr& task) -> ECode {

        auto linkTask = mCompiler->linkCommand(directory, program, objectFiles);
        task::gTPool->ensureScheduled(linkTask);
        EHTest(linkTask->join());
        EHEnd;
    };

    auto groupCb = std::make_shared<tpool::TaskCallback>(0, group, onFinish);
    task::gTPool->ensureScheduled(groupCb);
    return groupCb;
}

tpool::TaskSPtr CxxEngine::build(const doim::FsDirectorySPtr& directory,
                                 const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    auto crcTask =
        task::gManager->valid(std::make_shared<task::CxxProgramCrcTask>(cxxProgram));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish = [this, self, directory, program](
        const tpool::TaskSPtr& task) -> ECode {
        auto key = std::make_shared<doim::DbKey>("cxx_program:" + program->name());
        key = doim::gManager->unique(key);

        math::Crcsum crc;
        db::gDatabase->get(key->bytes(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxProgramCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Cxx program '{}' is already built.", program->name());
            EHEnd;
        }

        auto value = std::make_shared<doim::DbValue>(crcTask->crc());
        auto comTask = buildObjects(directory, program);
        auto updateTask = updateDbTask(comTask, key, value);
        task::gTPool->ensureScheduled(updateTask);
        EHTest(updateTask->join());
        EHEnd;
    };

    auto task = std::make_shared<tpool::TaskCallback>(0, crcTask, onFinish);
    task::gTPool->ensureScheduled(task);
    return task;
}
}
