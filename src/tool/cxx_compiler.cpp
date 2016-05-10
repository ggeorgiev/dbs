//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_compiler.h"
#include "task/manager.h"
#include "task/sys/parse_stdout_task.h"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "doim/sys/argument.h"
#include "doim/sys/command.h"
#include "rtti/class_rtti.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

namespace tool
{
doim::SysArgumentSPtr CxxCompiler::gFProfileArcArgument =
    doim::SysArgument::global("-fprofile-arcs", CxxCompiler::gFProfileArcArgument);
doim::SysArgumentSPtr CxxCompiler::gFTestCoverageArgument =
    doim::SysArgument::global("-ftest-coverage", CxxCompiler::gFTestCoverageArgument);

doim::SysArgumentSPtr CxxCompiler::gCoverageArgument =
    doim::SysArgument::global("--coverage", CxxCompiler::gCoverageArgument);

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

CxxCompiler::CxxCompiler(const doim::SysExecutableSPtr& compiler)
    : mCompiler(compiler)
{
}

tpool::TaskSPtr CxxCompiler::compileCommand(
    const doim::FsDirectorySPtr& directory,
    const doim::CxxObjectFileSPtr& objectFile) const
{
    auto compileArguments =
        includeArguments(directory, objectFile->cxxFile()->cxxIncludeDirectories());

    // compileArguments->insert(gFProfileArcArgument);
    // compileArguments->insert(gFTestCoverageArgument);

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

    auto id = rtti::RttiInfo<CxxCompiler, 0>::classId();
    return task::gManager->valid(
        std::make_shared<task::ParseStdoutTask>(compileCommand,
                                                objectFile->file()->directory(),
                                                id,
                                                task::ParseStdoutTask::logOnError(),
                                                "Compile " + file));
}

tpool::TaskSPtr CxxCompiler::linkCommand(
    const doim::FsDirectorySPtr& directory,
    const doim::FsDirectorySPtr& intermediate,
    const dom::CxxProgramSPtr& program,
    const doim::CxxObjectFileSetSPtr& objectFiles) const
{
    auto arguments = std::make_shared<doim::SysArgumentSet>();
    // arguments->insert(gCoverageArgument);

    auto argument_cxxflags = doim::gManager->obtainArgument("-std=c++11 -stdlib=libc++");
    arguments->insert(argument_cxxflags);

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

    for (const auto& objectFile : *objectFiles)
    {
        auto argument_obj =
            doim::gManager->obtainArgument(objectFile->file()->path(directory));
        arguments->insert(argument_obj);
    }

    auto argument_o = doim::gManager->obtainArgument("-o build/" + program->name());
    arguments->insert(argument_o);
    arguments = doim::gManager->unique(arguments);

    auto linkCommand = std::make_shared<doim::SysCommand>(mCompiler, arguments);
    linkCommand = doim::gManager->unique(linkCommand);

    auto id = rtti::RttiInfo<CxxCompiler, 1>::classId();
    return task::gManager->valid(
        std::make_shared<task::ParseStdoutTask>(linkCommand,
                                                intermediate,
                                                id,
                                                task::ParseStdoutTask::logOnError(),
                                                "Link " + program->name()));
}
}
