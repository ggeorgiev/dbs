//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "tool/cxx_compiler.h"
#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "task/sys/parse_stdout_task.h"
#include "task/tpool.h"
#include "tpool/task_callback.h"
#include "tpool/task_group.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include "log/log.h"
#include "rtti/class_rtti.hpp"
#include "math/crc.hpp"
#include <functional>
#include <str>
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

std::map<CxxEngine::EBuildFor, string>
    CxxEngine::gSubDirectory{{CxxEngine::EBuildFor::kDebug, "debug"},
                             {CxxEngine::EBuildFor::kRelease, "release"},
                             {CxxEngine::EBuildFor::kProfile, "profile"}};

std::map<CxxEngine::EBuildFor, doim::CxxProgram::EPurpose>
    CxxEngine::gProgramPurpose{{CxxEngine::EBuildFor::kDebug,
                                doim::CxxProgram::EPurpose::kDebug},
                               {CxxEngine::EBuildFor::kRelease,
                                doim::CxxProgram::EPurpose::kRelease},
                               {CxxEngine::EBuildFor::kProfile,
                                doim::CxxProgram::EPurpose::kProfile}};

bool CxxEngine::initDbKeyPurpose()
{
    CxxEngine::gDbKeyPurpose = {{CxxEngine::EBuildFor::kDebug, gDebugDbKey},
                                {CxxEngine::EBuildFor::kRelease, gReleaseDbKey},
                                {CxxEngine::EBuildFor::kProfile, gProfileDbKey}};
    return true;
}

std::map<CxxEngine::EBuildFor, doim::DbKeySPtr> CxxEngine::gDbKeyPurpose =
    im::InitializationManager::subscribe<
        std::map<CxxEngine::EBuildFor,
                 doim::DbKeySPtr>>(doim::Manager::object_rank() +
                                       im::InitializationManager::step() * 2,
                                   initDbKeyPurpose,
                                   nullptr);

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
        auto updateTask = task::DbPutTask::make(key, value);
        EHTest(updateTask->execute());
        EHEnd;
    };

    return tpool::TaskCallback::make(0, task, onFinish, nullptr);
}

tpool::TaskSPtr CxxEngine::compileTask(const doim::DbKeySPtr& ancenstor,
                                       const doim::FsDirectorySPtr& directory,
                                       const doim::CxxObjectFileSPtr& objectFile)
{
    auto crcTask = task::gManager->valid(task::CxxObjectFileCrcTask::make(objectFile));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, ancenstor, directory, objectFile](
            const tpool::TaskSPtr& task) -> ECode {

        auto key = doim::unique<doim::DbKey>(ancenstor, objectFile->file()->path());

        math::Crcsum crc;
        db::gDatabase->get(key->toString(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxObjectFileCrcTask>(task);
        if (crcTask->crc() != 0 && crcTask->crc() == crc)
        {
            DLOG("Cxx object '{}' is already built.",
                 objectFile->file()->path(directory));
            EHEnd;
        }

        auto compileCommand = mCompiler->compileCommand(directory, objectFile);

        auto id = rtti::RttiInfo<CxxEngine, 0>::classId();
        const string& description =
            "Compile " + objectFile->cxxFile()->file()->path(directory);
        auto compileTask = task::gManager->valid(
            task::ParseStdoutTask::make(compileCommand,
                                        objectFile->file()->directory(),
                                        id,
                                        task::ParseStdoutTask::logOnError(),
                                        description));

        task::gTPool->ensureScheduled(compileTask);

        auto value = doim::DbValue::make(crcTask->crc());
        auto compileCbTask = updateDbTask(compileTask, key, value);
        task::gTPool->ensureScheduled(compileCbTask);
        EHTest(compileCbTask->join());

        EHEnd;
    };

    auto task = tpool::TaskCallback::make(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::buildObjects(const doim::DbKeySPtr& ancenstor,
                                        const doim::FsDirectorySPtr& directory,
                                        const doim::CxxProgramSPtr& program)
{
    auto objectFileKey = doim::unique<doim::DbKey>(ancenstor, "object-file");

    std::vector<tpool::TaskSPtr> allTasks;
    for (const auto& objectFile : *program->cxxObjectFiles())
    {
        tpool::TaskSPtr task = compileTask(objectFileKey, directory, objectFile);
        allTasks.push_back(task);
    }

    auto group = tpool::TaskGroup::make(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, directory, program](const tpool::TaskSPtr& task) -> ECode {

        auto linkCommand = mCompiler->linkCommand(directory, program);

        auto id = rtti::RttiInfo<CxxEngine, 1>::classId();
        const string& description = "Link " + program->file()->path(directory);
        auto linkTask = task::gManager->valid(
            task::ParseStdoutTask::make(linkCommand,
                                        program->file()->directory(),
                                        id,
                                        task::ParseStdoutTask::logOnError(),
                                        description));

        task::gTPool->ensureScheduled(linkTask);
        EHTest(linkTask->join());
        EHEnd;
    };

    auto groupCb = tpool::TaskCallback::make(0, group, onFinish, nullptr);
    task::gTPool->ensureScheduled(groupCb);
    return groupCb;
}

tpool::TaskSPtr CxxEngine::build(EBuildFor buildFor,
                                 const doim::FsDirectorySPtr& directory,
                                 const dom::CxxProgramSPtr& program)
{
    const auto& build = doim::FsDirectory::obtain(directory, "build");
    const auto& intermediate = doim::FsDirectory::obtain(build, gSubDirectory[buildFor]);

    const auto& cxxProgram =
        program->cxxProgram(gProgramPurpose[buildFor], directory, intermediate);

    auto crcTask = task::gManager->valid(task::CxxProgramCrcTask::make(cxxProgram));
    task::gTPool->ensureScheduled(crcTask);

    auto ancestor = gDbKeyPurpose[buildFor];

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, ancestor, directory, cxxProgram](
            const tpool::TaskSPtr& task) -> ECode {

        auto cxxProgramKey = doim::unique<doim::DbKey>(ancestor, "cxx_program");

        const auto& path = cxxProgram->file()->path(directory);
        auto key = doim::unique<doim::DbKey>(cxxProgramKey, path);

        math::Crcsum crc;
        db::gDatabase->get(key->toString(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxProgramCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Cxx program '{}' is already built.", path);
            EHEnd;
        }

        auto value = doim::DbValue::make(crcTask->crc());
        auto comTask = buildObjects(ancestor, directory, cxxProgram);
        auto updateTask = updateDbTask(comTask, key, value);
        task::gTPool->ensureScheduled(updateTask);
        EHTest(updateTask->join());
        EHEnd;
    };

    auto task = tpool::TaskCallback::make(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::iwyuTask(const doim::FsDirectorySPtr& directory,
                                    const doim::CxxFileSPtr& cxxFile)
{
    auto crcTask = task::gManager->valid(task::CxxFileCrcTask::make(cxxFile));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, directory, cxxFile](const tpool::TaskSPtr& task) -> ECode {

        auto key = doim::unique<doim::DbKey>("iwyu:" + cxxFile->file()->path());

        math::Crcsum crc;
        db::gDatabase->get(key->toString(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxFileCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Iwyu '{}' is already checked.", cxxFile->file()->path(directory));
            EHEnd;
        }

        auto value = doim::DbValue::make(crcTask->crc());

        tpool::TaskCallback::Function onFinish =
            [key, value](const tpool::TaskSPtr&) -> ECode {
            auto updateTask = task::DbPutTask::make(key, value);
            EHTest(updateTask->execute());
            EHEnd;
        };

        auto iwyuTask = mIwyu->iwyuCommand(directory, cxxFile);
        task::gTPool->ensureScheduled(iwyuTask);

        auto iwyuCbTask = tpool::TaskCallback::make(0, iwyuTask, onFinish, nullptr);
        task::gTPool->ensureScheduled(iwyuCbTask);
        EHTest(iwyuCbTask->join());

        EHEnd;
    };

    auto task = tpool::TaskCallback::make(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::iwyu(const doim::FsDirectorySPtr& directory,
                                const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::FsDirectory::obtain(directory, "build");
    const auto& cxxProgram =
        program->cxxProgram(doim::CxxProgram::EPurpose::kDebug, directory, intermediate);

    const auto& objectFiles = cxxProgram->cxxObjectFiles();

    std::vector<tpool::TaskSPtr> allTasks;
    for (const auto& objectFile : *objectFiles)
    {
        tpool::TaskSPtr task = iwyuTask(directory, objectFile->cxxFile());
        allTasks.push_back(task);
    }

    auto group = tpool::TaskGroup::make(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);
    return group;
}
}
