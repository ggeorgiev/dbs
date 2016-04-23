//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_file_crc_task.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "task/sys/ensure_directory_task.h"
#include "task/sys/execute_command_task.h"
#include "task/tpool.h"
#include "tpool/task_group.h"
#include "tpool/task_sequence.h"
#include "tool/cxx_compiler.h"
#include "dom/cxx/cxx_library.hpp"
#include "dom/cxx/cxx_program.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_program.h"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.h"
#include "db/database.h"
#include "err/err_cppformat.h"
#include "math/crc.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

namespace tool
{
CxxCompiler::CxxCompiler(const doim::SysExecutableSPtr& compiler)
    : mCompiler(compiler)
{
}

doim::SysArgumentSetSPtr CxxCompiler::includeArguments(
    const doim::FsDirectorySPtr& directory,
    const doim::CxxIncludeDirectorySetSPtr& includeDirectories)
{
    auto arguments = std::make_shared<doim::SysArgumentSet>();

    for (const auto& includeDirectory : *includeDirectories)
    {
        std::string value;
        switch (includeDirectory->type())
        {
            case doim::CxxIncludeDirectory::Type::kUser:
                value = "-I ";
                break;
            case doim::CxxIncludeDirectory::Type::kSystem:
                value = "-isystem ";
                break;
        }

        value += includeDirectory->directory()->path(directory);

        arguments->insert(doim::gManager->obtainArgument(value));
    }

    return arguments;
}

ECode CxxCompiler::compileTask(const doim::FsDirectorySPtr& directory,
                               const doim::CxxObjectFileSPtr& objectFile,
                               tpool::TaskSPtr& task)
{
    auto crcTask = task::gManager->valid(
        std::make_shared<task::CxxFileCrcTask>(objectFile->cxxFile()));
    task::gTPool->ensureScheduled(crcTask);
    EHTest(crcTask->join(), objectFile->file()->path());

    auto key = std::make_shared<doim::DbKey>("file:" + objectFile->file()->path());
    key = doim::gManager->unique(key);

    math::Crcsum crc;
    db::gDatabase->get(key->bytes(), crc);

    if (crcTask->crc() == crc)
    {
        task.reset();
        EHEnd;
    }

    auto mkdirTask = task::gManager->valid(
        std::make_shared<task::EnsureDirectoryTask>(objectFile->file()->directory()));

    auto compileArguments =
        includeArguments(directory, objectFile->cxxFile()->cxxIncludeDirectories());

    auto argument_cxxflags = doim::gManager->obtainArgument(
        "-std=c++11 -stdlib=libc++ -O0 -g "
        "-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/"
        "MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk");
    compileArguments->insert(argument_cxxflags);

    const std::string& file = objectFile->cxxFile()->file()->path(directory);

    auto argument_c = doim::gManager->obtainArgument("-c " + file);
    compileArguments->insert(argument_c);

    auto argument_o =
        doim::gManager->obtainArgument("-o " + objectFile->file()->path(directory));
    compileArguments->insert(argument_o);
    compileArguments = doim::gManager->unique(compileArguments);

    auto compileCommand = std::make_shared<doim::SysCommand>(mCompiler, compileArguments);
    compileCommand = doim::gManager->unique(compileCommand);
    auto compileTask = task::gManager->valid(
        std::make_shared<task::ExecuteCommandTask>(compileCommand, "Compile " + file));

    auto value = std::make_shared<doim::DbValue>(crcTask->crc());
    auto updateTask =
        task::gManager->valid(std::make_shared<task::DbPutTask>(key, value));

    auto tasks = std::vector<tpool::TaskSPtr>{mkdirTask, compileTask, updateTask};
    task = task::gManager->unique(std::make_shared<tpool::TaskSequence>(0, tasks));

    EHEnd;
}

ECode CxxCompiler::commands(const doim::FsDirectorySPtr& directory,
                            const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    auto arguments = std::make_shared<doim::SysArgumentSet>();
    for (const auto& cxxLibrary : program->recursiveCxxLibraries())
    {
        if (cxxLibrary->binary() != nullptr)
        {
            auto argument_L = doim::gManager->obtainArgument(
                "-L " + cxxLibrary->binary()->directory()->path(directory));
            arguments->insert(argument_L);

            std::string name = cxxLibrary->binary()->name();
            auto argument_l =
                doim::gManager->obtainArgument("-l" + name.substr(3, name.size() - 5));
            arguments->insert(argument_l);
        }
    }

    const auto& objectFiles = cxxProgram->cxxObjectFiles();

    std::vector<tpool::TaskSPtr> allTasks;
    for (const auto& objectFile : *objectFiles)
    {
        auto argument_obj =
            doim::gManager->obtainArgument(objectFile->file()->path(directory));
        arguments->insert(argument_obj);

        tpool::TaskSPtr task;
        EHTest(compileTask(directory, objectFile, task));

        if (task != nullptr)
            allTasks.push_back(task);
    }

    auto group = std::make_shared<tpool::TaskGroup>(task::gTPool, 0, allTasks);
    task::gTPool->ensureScheduled(group);
    EHTest(group->join());

    auto argument_o = doim::gManager->obtainArgument("-o build/" + program->name());
    arguments->insert(argument_o);
    arguments = doim::gManager->unique(arguments);

    auto linkCommand = std::make_shared<doim::SysCommand>(mCompiler, arguments);
    linkCommand = doim::gManager->unique(linkCommand);

    auto linkTask = task::gManager->valid(
        std::make_shared<task::ExecuteCommandTask>(linkCommand,
                                                   "Link " + program->name()));
    task::gTPool->ensureScheduled(linkTask);

    EHTest(linkTask->join());
    EHEnd;
}
}
