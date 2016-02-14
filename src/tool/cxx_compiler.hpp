//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_file_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/sys/ensure_directory_task.h"
#include "task/sys/execute_command_task.h"
#include "dom/cxx/cxx_program.hpp"
#include "doim/cxx/cxx_file.hpp"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.hpp"
#include "db/database.h"
#include <memory>
#include <set>
#include <sstream>
#include <string>

namespace tool
{
class CxxCompiler;
typedef std::shared_ptr<CxxCompiler> CxxCompilerSPtr;

class CxxCompiler
{
public:
    CxxCompiler(const doim::SysExecutableSPtr& compiler)
        : mCompiler(compiler)
    {
    }

    doim::SysArgumentSetSPtr includeArguments(
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

    ECode tasks(const doim::FsDirectorySPtr& directory,
                const doim::CxxObjectFileSPtr& objectFile,
                std::vector<tpool::TaskSPtr>& tasks)
    {
        auto task = std::make_shared<task::CxxFileCrcTask>(objectFile->cxxFile());
        EHTest((*task)(), objectFile->file()->path());

        auto key = std::make_shared<doim::DbKey>("file:" +
                                                 objectFile->cxxFile()->file()->path());
        key = doim::gManager->unique(key);

        uint64_t crc;
        db::gDatabase->get(key->bytes(), crc);

        if (task->crc() == crc)
            EHEnd;

        std::cout << "set crc:" << crc << " expected: " << task->crc() << std::endl;

        auto mkdirTask =
            std::make_shared<task::EnsureDirectoryTask>(objectFile->file()->directory());
        tasks.push_back(mkdirTask);

        auto compileArguments =
            includeArguments(directory, objectFile->cxxFile()->cxxIncludeDirectories());

        auto argument_cxxflags = doim::gManager->obtainArgument(
            "-std=c++11 -stdlib=libc++ -O0 -g "
            "-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/"
            "MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk");
        compileArguments->insert(argument_cxxflags);

        auto argument_c = doim::gManager->obtainArgument(
            "-c " + objectFile->cxxFile()->file()->path(directory));
        compileArguments->insert(argument_c);

        auto argument_o =
            doim::gManager->obtainArgument("-o " + objectFile->file()->path(directory));
        compileArguments->insert(argument_o);

        auto compileCommand =
            std::make_shared<doim::SysCommand>(mCompiler, compileArguments);
        auto compileTask = std::make_shared<task::ExecuteCommandTask>(compileCommand);
        tasks.push_back(compileTask);

        auto value = std::make_shared<doim::DbValue>(task->crc());
        auto updateTask = std::make_shared<task::DbPutTask>(key, value);
        tasks.push_back(updateTask);

        EHEnd;
    }

    ECode commands(const doim::FsDirectorySPtr& directory,
                   const dom::CxxProgramSPtr& program,
                   std::string& cmd)
    {
        const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");

        auto arguments = std::make_shared<doim::SysArgumentSet>();
        for (const auto& cxxLibrary : program->recursiveCxxLibraries())
        {
            if (cxxLibrary->binary() != nullptr)
            {
                auto argument_L = doim::gManager->obtainArgument(
                    "-L " + cxxLibrary->binary()->directory()->path(directory));
                arguments->insert(argument_L);

                std::string name = cxxLibrary->binary()->name();
                auto argument_l = doim::gManager->obtainArgument(
                    "-l" + name.substr(3, name.size() - 5));
                arguments->insert(argument_l);
            }
            else
            {
                const auto& objectFiles =
                    cxxLibrary->cxxObjectFiles(directory, intermediate);

                for (const auto& objectFile : objectFiles)
                {
                    auto argument_obj = doim::gManager->obtainArgument(
                        objectFile->file()->path(directory));
                    arguments->insert(argument_obj);

                    std::vector<tpool::TaskSPtr> tsks;
                    EHTest(tasks(directory, objectFile, tsks));
                    for (auto task : tsks)
                    {
                        ILOG("RUN: " + task->description());
                        EHTest((*task)());
                        ILOG("DONE: " + task->description());
                    }
                }
            }
        }

        const auto& objectFiles = program->cxxObjectFiles(directory, intermediate);
        for (const auto& objectFile : objectFiles)
        {
            auto argument_obj =
                doim::gManager->obtainArgument(objectFile->file()->path(directory));
            arguments->insert(argument_obj);

            std::vector<tpool::TaskSPtr> tsks;
            EHTest(tasks(directory, objectFile, tsks));
            for (auto task : tsks)
            {
                ILOG("RUN: " + task->description());
                EHTest((*task)());
                ILOG("DONE: " + task->description());
            }
        }

        auto argument_o = doim::gManager->obtainArgument("-o build/" + program->name());
        arguments->insert(argument_o);

        auto linkCommand = std::make_shared<doim::SysCommand>(mCompiler, arguments);

        auto linkTask = std::make_shared<task::ExecuteCommandTask>(linkCommand);

        ILOG("RUN: " + linkTask->description());
        EHTest((*linkTask)());
        ILOG("DONE: " + linkTask->description());
        EHEnd;
    }

private:
    doim::SysExecutableSPtr mCompiler;
};
}
