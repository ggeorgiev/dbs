//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "tool/cxx_compiler.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "task/tpool.h"
#include "tpool/task_callback.h"
#include "tpool/task_group.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err.h"
#include "log/log.h"
#include "math/crc.hpp"
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace engine
{
doim::DbKeySPtr CxxEngine::gBuildDbKey =
    doim::DbKey::global("build", CxxEngine::gBuildDbKey);
doim::DbKeySPtr CxxEngine::gDebugDbKey =
    doim::DbKey::global(gBuildDbKey, 1, "debug", CxxEngine::gDebugDbKey);
doim::DbKeySPtr CxxEngine::gReleaseDbKey =
    doim::DbKey::global(gBuildDbKey, 1, "release", CxxEngine::gReleaseDbKey);
doim::DbKeySPtr CxxEngine::gProfileDbKey =
    doim::DbKey::global(gBuildDbKey, 1, "profile", CxxEngine::gProfileDbKey);

CxxEngine::CxxEngine(const tool::CxxClangFormatSPtr& formatter,
                     const tool::CxxCompilerSPtr& compiler,
                     const tool::CxxIwyuSPtr& iwyu)
    : mFormatter(formatter)
    , mCompiler(compiler)
    , mIwyu(iwyu)
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

    return std::make_shared<tpool::TaskCallback>(0, task, onFinish, nullptr);
}

tpool::TaskSPtr CxxEngine::compileTask(const doim::SysArgumentSet& arguments,
                                       const doim::DbKeySPtr& ancenstor,
                                       const doim::FsDirectorySPtr& directory,
                                       const doim::CxxObjectFileSPtr& objectFile)
{
    auto crcTask =
        task::gManager->valid(std::make_shared<task::CxxObjectFileCrcTask>(objectFile));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, arguments, ancenstor, directory, objectFile](
            const tpool::TaskSPtr& task) -> ECode {

        auto key = std::make_shared<doim::DbKey>(ancenstor, objectFile->file()->path());
        key = doim::gManager->unique(key);

        math::Crcsum crc;
        db::gDatabase->get(key->string(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxObjectFileCrcTask>(task);
        if (crcTask->crc() != 0 && crcTask->crc() == crc)
        {
            DLOG("Cxx object '{}' is already built.",
                 objectFile->file()->path(directory));
            EHEnd;
        }

        auto compileTask = mCompiler->compileCommand(arguments, directory, objectFile);
        task::gTPool->ensureScheduled(compileTask);

        auto value = std::make_shared<doim::DbValue>(crcTask->crc());
        auto compileCbTask = updateDbTask(compileTask, key, value);
        task::gTPool->ensureScheduled(compileCbTask);
        EHTest(compileCbTask->join());

        EHEnd;
    };

    auto task = std::make_shared<tpool::TaskCallback>(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::buildObjects(CxxEngine::EBuildFor buildFor,
                                        const doim::DbKeySPtr& ancenstor,
                                        const doim::FsDirectorySPtr& directory,
                                        const doim::FsDirectorySPtr& intermediate,
                                        const dom::CxxProgramSPtr& program)
{
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    const auto& objectFiles = cxxProgram->cxxObjectFiles();

    auto objectFileKey = std::make_shared<doim::DbKey>(ancenstor, "object-file");
    objectFileKey = doim::gManager->unique(objectFileKey);

    std::vector<tpool::TaskSPtr> allTasks;
    for (const auto& objectFile : *objectFiles)
    {
        tpool::TaskSPtr task =
            compileTask(compileArguments(buildFor), objectFileKey, directory, objectFile);
        allTasks.push_back(task);
    }

    auto group = std::make_shared<tpool::TaskGroup>(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, buildFor, directory, intermediate, program, objectFiles](
            const tpool::TaskSPtr& task) -> ECode {

        auto linkTask = mCompiler->linkCommand(linkArguments(buildFor),
                                               directory,
                                               intermediate,
                                               program,
                                               objectFiles);
        task::gTPool->ensureScheduled(linkTask);
        EHTest(linkTask->join());
        EHEnd;
    };

    auto groupCb = std::make_shared<tpool::TaskCallback>(0, group, onFinish, nullptr);
    task::gTPool->ensureScheduled(groupCb);
    return groupCb;
}

std::string CxxEngine::subdirectory(CxxEngine::EBuildFor buildFor)
{
    switch (buildFor)
    {
        case EBuildFor::kDebug:
            return "debug";
            break;
        case EBuildFor::kRelease:
            return "release";
            break;
        case EBuildFor::kProfile:
            return "profile";
            break;
    }

    return "";
}

doim::DbKeySPtr CxxEngine::dbKey(CxxEngine::EBuildFor buildFor)
{
    switch (buildFor)
    {
        case EBuildFor::kDebug:
            return gDebugDbKey;
            break;
        case EBuildFor::kRelease:
            return gReleaseDbKey;
            break;
        case EBuildFor::kProfile:
            return gProfileDbKey;
            break;
    }

    return nullptr;
}

doim::SysArgumentSet CxxEngine::compileArguments(CxxEngine::EBuildFor buildFor) const
{
    switch (buildFor)
    {
        case EBuildFor::kDebug:
            return {};
            break;
        case EBuildFor::kRelease:
            return {};
            break;
        case EBuildFor::kProfile:
            return mCompiler->compileProfileArguments();
            break;
    }

    return {};
}

doim::SysArgumentSet CxxEngine::linkArguments(CxxEngine::EBuildFor buildFor) const
{
    switch (buildFor)
    {
        case EBuildFor::kDebug:
            return {};
            break;
        case EBuildFor::kRelease:
            return {};
            break;
        case EBuildFor::kProfile:
            return mCompiler->linkProfileArguments();
            break;
    }

    return {};
}

tpool::TaskSPtr CxxEngine::build(EBuildFor buildFor,
                                 const doim::FsDirectorySPtr& directory,
                                 const dom::CxxProgramSPtr& program)
{
    const auto& build = doim::gManager->obtainDirectory(directory, "build");
    const auto& intermediate =
        doim::gManager->obtainDirectory(build, subdirectory(buildFor));

    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    auto crcTask =
        task::gManager->valid(std::make_shared<task::CxxProgramCrcTask>(cxxProgram));
    task::gTPool->ensureScheduled(crcTask);

    auto ancestor = dbKey(buildFor);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, buildFor, ancestor, directory, intermediate, program](
            const tpool::TaskSPtr& task) -> ECode {

        auto cxxProgramKey = doim::gManager->unique(
            std::make_shared<doim::DbKey>(ancestor, "cxx_program"));
        auto key = doim::gManager->unique(
            std::make_shared<doim::DbKey>(cxxProgramKey, program->name()));

        math::Crcsum crc;
        db::gDatabase->get(key->string(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxProgramCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Cxx program '{}' is already built.", program->name());
            EHEnd;
        }

        auto value = std::make_shared<doim::DbValue>(crcTask->crc());
        auto comTask = buildObjects(buildFor, ancestor, directory, intermediate, program);
        auto updateTask = updateDbTask(comTask, key, value);
        task::gTPool->ensureScheduled(updateTask);
        EHTest(updateTask->join());
        EHEnd;
    };

    auto task = std::make_shared<tpool::TaskCallback>(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::iwyuTask(const doim::FsDirectorySPtr& directory,
                                    const doim::CxxFileSPtr& cxxFile)
{
    auto crcTask = task::gManager->valid(std::make_shared<task::CxxFileCrcTask>(cxxFile));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, directory, cxxFile](const tpool::TaskSPtr& task) -> ECode {

        auto key = std::make_shared<doim::DbKey>("iwyu:" + cxxFile->file()->path());
        key = doim::gManager->unique(key);

        math::Crcsum crc;
        db::gDatabase->get(key->string(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxFileCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Iwyu '{}' is already checked.", cxxFile->file()->path(directory));
            EHEnd;
        }

        auto value = std::make_shared<doim::DbValue>(crcTask->crc());

        tpool::TaskCallback::Function onFinish =
            [key, value](const tpool::TaskSPtr&) -> ECode {
            auto updateTask = std::make_shared<task::DbPutTask>(key, value);
            EHTest(updateTask->execute());
            EHEnd;
        };

        auto iwyuTask = mIwyu->iwyuCommand(directory, cxxFile);
        task::gTPool->ensureScheduled(iwyuTask);

        auto iwyuCbTask =
            std::make_shared<tpool::TaskCallback>(0, iwyuTask, onFinish, nullptr);
        task::gTPool->ensureScheduled(iwyuCbTask);
        EHTest(iwyuCbTask->join());

        EHEnd;
    };

    auto task = std::make_shared<tpool::TaskCallback>(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::iwyu(const doim::FsDirectorySPtr& directory,
                                const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    const auto& objectFiles = cxxProgram->cxxObjectFiles();

    std::vector<tpool::TaskSPtr> allTasks;
    for (const auto& objectFile : *objectFiles)
    {
        tpool::TaskSPtr task = iwyuTask(directory, objectFile->cxxFile());
        allTasks.push_back(task);
    }

    auto group = std::make_shared<tpool::TaskGroup>(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);
    return group;
}
}
