//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "tool/cxx/cxx_compiler.h"
#include "task/cxx/cxx_object_file_crc_task.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/cxx/cxx_source_crc_task.h"
#include "task/cxx/cxx_source_headers_task.h"
#include "task/db/db_put_task.h"
#include "task/protobuf/protobuf_file_crc_task.h"
#include "task/sys/parse_stdout_task.h"
#include "task/tpool.h"
#include "tpool/task_callback.h"
#include "tpool/task_group.h"
#include "logex/log.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_program.h"
#include "doim/fs/fs_file.h"
#include "doim/protobuf/protobuf_file.h"
#include "doim/sys/sys_command.h"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include "db/database.h"
#include "err/err.h"
#include "err/err_assert.h"
#include "im/initialization_manager.hpp"
#include "math/crc.hpp"
#include "rtti/class_rtti.hpp"
#include <algorithm>
#include <functional>
#include <str>
#include <unordered>
#include <variant>
#include <vector>

namespace engine
{
doim::DbKeySPtr CxxEngine::gBuildDbKey =
    doim::DbKey::global(doim::DbKey::null, "build", CxxEngine::gBuildDbKey);
doim::DbKeySPtr CxxEngine::gDebugDbKey =
    doim::DbKey::global<2>(gBuildDbKey, "debug", CxxEngine::gDebugDbKey);
doim::DbKeySPtr CxxEngine::gReleaseDbKey =
    doim::DbKey::global<2>(gBuildDbKey, "release", CxxEngine::gReleaseDbKey);
doim::DbKeySPtr CxxEngine::gProfileDbKey =
    doim::DbKey::global<2>(gBuildDbKey, "profile", CxxEngine::gProfileDbKey);

std::map<CxxEngine::EBuildFor, string> CxxEngine::gSubDirectory{
    {CxxEngine::EBuildFor::kDebug, "debug"},
    {CxxEngine::EBuildFor::kRelease, "release"},
    {CxxEngine::EBuildFor::kProfile, "profile"}};

std::map<CxxEngine::EBuildFor, doim::CxxProgram::EPurpose> CxxEngine::gProgramPurpose{
    {CxxEngine::EBuildFor::kDebug, doim::CxxProgram::EPurpose::kDebug},
    {CxxEngine::EBuildFor::kRelease, doim::CxxProgram::EPurpose::kRelease},
    {CxxEngine::EBuildFor::kProfile, doim::CxxProgram::EPurpose::kProfile}};

bool CxxEngine::initDbKeyPurpose()
{
    CxxEngine::gDbKeyPurpose = {{CxxEngine::EBuildFor::kDebug, gDebugDbKey},
                                {CxxEngine::EBuildFor::kRelease, gReleaseDbKey},
                                {CxxEngine::EBuildFor::kProfile, gProfileDbKey}};
    return true;
}

std::map<CxxEngine::EBuildFor, doim::DbKeySPtr> CxxEngine::gDbKeyPurpose =
    im::InitializationManager::subscribe<std::map<CxxEngine::EBuildFor, doim::DbKeySPtr>>(
        CxxEngine::rank(), initDbKeyPurpose, nullptr);

CxxEngine::CxxEngine(const tool::CxxClangFormatSPtr& formatter,
                     const tool::CxxCompilerSPtr& compiler,
                     const tool::CxxIwyuSPtr& iwyu,
                     const tool::ProtobufCompilerSPtr& protobufCompiler)
    : mFormatter(formatter)
    , mCompiler(compiler)
    , mIwyu(iwyu)
    , mProtobufCompiler(protobufCompiler)
{
}

template <typename T>
tpool::TaskSPtr CxxEngine::updateDbTask(const tpool::TaskSPtr& task,
                                        const doim::DbKeySPtr& key,
                                        const T& crcTask) const
{
    auto onFinish = [crcTask, key](const tpool::TaskSPtr&) -> ECode {
        auto task = crcTask->rerun();
        task::gTPool->ensureScheduled(task);
        EHTest(task->join());

        doim::TagSetSPtr logTags = doim::TagSet::make(*task->tags());
        logTags->erase(doim::gTaskTag);
        logTags = doim::TagSet::unique(logTags);

        if (crcTask->crc() != task->crc())
        {
            LOGEX(logTags,
                  "The crc for key {} changed {:x} -> {:x}",
                  key->toString(),
                  crcTask->crc(),
                  task->crc());
        }

        auto value = doim::DbValue::make(task->crc());
        auto updateTask = task::DbPutTask::make(key, value);
        EHTest(updateTask->execute());
        EHEnd;
    };

    return tpool::TaskCallback::make(0, task, onFinish, nullptr);
}

tpool::TaskSPtr CxxEngine::compileOrigin(const doim::DbKeySPtr& ancestor,
                                         const doim::CxxSourceOriginSPtr& origin,
                                         const doim::FsDirectorySPtr& directory)
{
    if (apply_visitor(vst::isNullptr, origin))
        return nullptr;

    if (origin.type() == typeid(doim::ProtobufFileSPtr))
    {
        const auto& protobufCxxKey = doim::DbKey::unique(ancestor, "protobuf-file:cxx");

        const auto& protobufFile = boost::get<doim::ProtobufFileSPtr>(origin);

        auto crcTask = task::ProtobufFileCrcTask::valid(protobufFile);
        task::gTPool->ensureScheduled(crcTask);

        auto self = shared_from_this();
        tpool::TaskCallback::Function onFinish =
            [this, self, protobufCxxKey, directory, protobufFile](
                const tpool::TaskSPtr& task) -> ECode {

            auto key = doim::DbKey::unique(protobufCxxKey, protobufFile->file()->path());

            math::Crcsum crc;
            db::gDatabase->get(key->toString(), crc);

            auto crcTask = std::static_pointer_cast<task::ProtobufFileCrcTask>(task);
            if (crcTask->crc() != 0 && crcTask->crc() == crc)
            {
                DLOG("Cxx artifacts for protobuf '{}' are already built.",
                     protobufFile->file()->path(directory));
                EHEnd;
            }

            auto compileProtobufCommand =
                mProtobufCompiler->compileCommand(protobufFile->directory(),
                                                  protobufFile);

            auto id = rtti::RttiInfo<CxxEngine, __COUNTER__>::classId();
            const string& description =
                "Compile " + protobufFile->file()->path(directory);

            auto compileTask =
                task::ParseStdoutTask::valid(compileProtobufCommand,
                                             protobufFile->directory(),
                                             id,
                                             task::ParseStdoutTask::logOnError(),
                                             description);

            task::gTPool->ensureScheduled(compileTask);

            auto compileCbTask = updateDbTask(compileTask, key, crcTask);
            task::gTPool->ensureScheduled(compileCbTask);
            EHTest(compileCbTask->join());

            EHEnd;
        };

        auto task = tpool::TaskCallback::make(0, crcTask, onFinish, nullptr);
        task::gTPool->ensureScheduled(task);
        return task;
    }

    ASSERT(false);
    return nullptr;
}

tpool::TaskSPtr CxxEngine::compileTask(const doim::DbKeySPtr& ancestor,
                                       const doim::FsDirectorySPtr& directory,
                                       const doim::CxxObjectFileSPtr& objectFile)
{
    auto crcTask = task::CxxObjectFileCrcTask::valid(objectFile);
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, ancestor, directory, objectFile](
            const tpool::TaskSPtr& task) -> ECode {

        auto key = doim::DbKey::unique(ancestor, objectFile->cxxFile()->file()->path());

        math::Crcsum crc;
        db::gDatabase->get(key->toString(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxObjectFileCrcTask>(task);
        if (crcTask->crc() != 0 && crcTask->crc() == crc)
        {
            DLOG("Cxx object '{}' is already built.",
                 objectFile->file()->path(directory));
            EHEnd;
        }

        std::vector<tpool::TaskSPtr> allTasks;

        const auto& originTask = compileOrigin(ancestor->ancestor(),
                                               objectFile->cxxFile()->origin(),
                                               directory);
        if (originTask != nullptr)
            allTasks.push_back(originTask);

        auto headersTask =
            task::CxxSourceHeadersTask::valid(task::CxxSourceHeadersTask::EDepth::kAll,
                                              objectFile->cxxFile(),
                                              nullptr);
        task::gTPool->ensureScheduled(headersTask);
        EHTest(headersTask->join());

        auto headersInfo = headersTask->headersInfo();
        for (const auto& headerInfo : headersInfo)
        {
            const auto& originTask = compileOrigin(ancestor->ancestor(),
                                                   headerInfo.mHeader->origin(),
                                                   directory);
            if (originTask != nullptr)
                allTasks.push_back(originTask);
        }

        if (!allTasks.empty())
        {
            auto group = tpool::TaskGroup::make(task::gTPool, 0, allTasks);
            task::gTPool->ensureScheduled(group);
            EHTest(group->join());
        }

        auto compileCommand = mCompiler->compileCommand(directory, objectFile);

        auto id = rtti::RttiInfo<CxxEngine, __COUNTER__>::classId();
        const string& description =
            "Compile " + objectFile->cxxFile()->file()->path(directory);
        auto compileTask =
            task::ParseStdoutTask::valid(compileCommand,
                                         objectFile->file()->directory(),
                                         id,
                                         task::ParseStdoutTask::logOnError(),
                                         description);

        task::gTPool->ensureScheduled(compileTask);

        auto compileCbTask = updateDbTask(compileTask, key, crcTask);
        task::gTPool->ensureScheduled(compileCbTask);
        EHTest(compileCbTask->join());

        EHEnd;
    };

    auto task = tpool::TaskCallback::make(0, crcTask, onFinish, nullptr);
    task::gTPool->ensureScheduled(task);
    return task;
}

tpool::TaskSPtr CxxEngine::compileObjects(const doim::DbKeySPtr& ancestor,
                                          const doim::FsDirectorySPtr& directory,
                                          const doim::CxxProgramSPtr& program)
{
    auto objectFileKey = doim::DbKey::unique(ancestor, "object-file");

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

        auto id = rtti::RttiInfo<CxxEngine, __COUNTER__>::classId();
        const string& description = "Link " + program->file()->path(directory);
        auto linkTask = task::ParseStdoutTask::valid(linkCommand,
                                                     program->file()->directory(),
                                                     id,
                                                     task::ParseStdoutTask::logOnError(),
                                                     description);

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

    auto crcTask = task::CxxProgramCrcTask::valid(cxxProgram);
    task::gTPool->ensureScheduled(crcTask);

    auto ancestor = gDbKeyPurpose[buildFor];

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, ancestor, directory, cxxProgram](
            const tpool::TaskSPtr& task) -> ECode {

        auto cxxProgramKey = doim::DbKey::unique(ancestor, "cxx_program");

        const auto& path = cxxProgram->file()->path(directory);
        auto key = doim::DbKey::unique(cxxProgramKey, path);

        math::Crcsum crc;
        db::gDatabase->get(key->toString(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxProgramCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Cxx program '{}' is already built.", path);
            EHEnd;
        }

        auto comTask = compileObjects(ancestor, directory, cxxProgram);
        auto updateTask = updateDbTask(comTask, key, crcTask);
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
    auto crcTask = task::CxxSourceCrcTask::valid(task::CxxSourceCrcTask::EDepth::kAll,
                                                 cxxFile,
                                                 nullptr);
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish =
        [this, self, directory, cxxFile](const tpool::TaskSPtr& task) -> ECode {

        auto key = doim::DbKey::unique(nullptr, "iwyu:" + cxxFile->file()->path());

        math::Crcsum crc;
        db::gDatabase->get(key->toString(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxSourceCrcTask>(task);
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
        // Do not check generated files
        if (!apply_visitor(vst::isNullptr, objectFile->cxxFile()->origin()))
            continue;

        tpool::TaskSPtr task = iwyuTask(directory, objectFile->cxxFile());
        allTasks.push_back(task);
    }

    auto group = tpool::TaskGroup::make(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);
    return group;
}

static string makeDirectory(const doim::FsDirectorySPtr& directory,
                            const doim::FsDirectorySPtr& target,
                            unordered_set<doim::FsDirectorySPtr>& directories)
{
    std::stringstream stream;
    while (directories.find(target) == directories.end())
    {
        auto current = target;
        while (directories.find(current->parent()) == directories.end())
        {
            current = current->parent();
        }
        directories.insert(current);
        if (current != directory)
        {
            stream << "if [ ! -e " << current->path(directory) << " ]; then mkdir "
                   << current->path(directory) << "; fi" << std::endl;
        }
    }
    return stream.str();
}

string CxxEngine::buildScript(EBuildFor buildFor,
                              const doim::FsDirectorySPtr& directory,
                              const dom::CxxProgramSPtr& program)
{
    std::stringstream stream;

    const auto& build = doim::FsDirectory::obtain(directory, "build");
    const auto& intermediate = doim::FsDirectory::obtain(build, gSubDirectory[buildFor]);

    const auto& cxxProgram =
        program->cxxProgram(gProgramPurpose[buildFor], directory, intermediate);

    unordered_set<doim::FsDirectorySPtr> directories;
    directories.insert(directory);

    auto files = cxxProgram->cxxObjectFiles();
    std::vector<doim::CxxObjectFileSPtr> objects;
    objects.insert(objects.begin(), files->begin(), files->end());

    std::sort(objects.begin(),
              objects.end(),
              [](const doim::CxxObjectFileSPtr& a, const doim::CxxObjectFileSPtr& b) {
                  return a->file()->path() < b->file()->path();
              });

    for (const auto& objectFile : objects)
    {
        string compileOrigin;

        auto origin = objectFile->cxxFile()->origin();
        if (apply_visitor(vst::isNullptr, origin))
            continue;

        string description;
        doim::SysCommandSPtr compileCommand;

        if (origin.type() == typeid(doim::ProtobufFileSPtr))
        {
            const auto& protobufFile = boost::get<doim::ProtobufFileSPtr>(origin);
            description = protobufFile->file()->path(directory);
            compileCommand = mProtobufCompiler->compileCommand(protobufFile->directory(),
                                                               protobufFile);
        }
        else
            ASSERT(false);

        stream << makeDirectory(directory, objectFile->file()->directory(), directories);
        stream << "echo Compile " << description << std::endl;
        stream << compileCommand->toString(directory) << " &" << std::endl;
    }

    stream << "wait" << std::endl;

    for (const auto& objectFile : objects)
    {
        stream << makeDirectory(directory, objectFile->file()->directory(), directories);
        stream << "echo Compile " << objectFile->cxxFile()->file()->path(directory)
               << std::endl;
        auto compileCommand = mCompiler->compileCommand(directory, objectFile);
        stream << compileCommand->toString(directory) << " &" << std::endl;
    }

    stream << "wait" << std::endl;

    auto linkCommand = mCompiler->linkCommand(directory, cxxProgram);
    stream << makeDirectory(directory, cxxProgram->file()->directory(), directories);
    stream << "echo Link " << cxxProgram->file()->path(directory) << std::endl;
    stream << linkCommand->toString(directory) << std::endl;

    return stream.str();
}
}
