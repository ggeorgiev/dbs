//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_compiler.h"
#include "task/manager.h"
#include "task/sys/parse_stdout_task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_static_library.h"
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
doim::SysArgumentSPtr CxxCompiler::gOptimizationLevel0Argument =
    doim::SysArgument::global("-O0", CxxCompiler::gOptimizationLevel0Argument);
doim::SysArgumentSPtr CxxCompiler::gDebuggingInformationArgument =
    doim::SysArgument::global("-g", CxxCompiler::gDebuggingInformationArgument);
doim::SysArgumentSPtr CxxCompiler::gStdCpp11Argument =
    doim::SysArgument::global("-std=c++11", CxxCompiler::gStdCpp11Argument);
doim::SysArgumentSPtr CxxCompiler::gFProfileArcArgument =
    doim::SysArgument::global("-fprofile-arcs", CxxCompiler::gFProfileArcArgument);
doim::SysArgumentSPtr CxxCompiler::gFTestCoverageArgument =
    doim::SysArgument::global("-ftest-coverage", CxxCompiler::gFTestCoverageArgument);

doim::SysArgumentSPtr CxxCompiler::gCoverageArgument =
    doim::SysArgument::global("--coverage", CxxCompiler::gCoverageArgument);
doim::SysArgumentSPtr CxxCompiler::gStdLibc11Argument =
    doim::SysArgument::global("-stdlib=libc++", CxxCompiler::gStdLibc11Argument);

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
            case doim::CxxIncludeDirectory::EType::kUser:
                value = "-I ";
                break;
            case doim::CxxIncludeDirectory::EType::kSystem:
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

doim::SysCommandSPtr CxxCompiler::compileCommand(
    const doim::FsDirectorySPtr& directory,
    const doim::CxxObjectFileSPtr& objectFile) const
{
    auto compileArguments =
        includeArguments(directory, objectFile->cxxFile()->cxxIncludeDirectories());
    compileArguments->insert(gStdCpp11Argument);

    switch (objectFile->purpose())
    {
        case doim::CxxObjectFile::EPurpose::kDebug:
            compileArguments->insert(gOptimizationLevel0Argument);
            compileArguments->insert(gDebuggingInformationArgument);
            break;
        case doim::CxxObjectFile::EPurpose::kRelease:
            break;
        case doim::CxxObjectFile::EPurpose::kProfile:
            compileArguments->insert(gFProfileArcArgument);
            compileArguments->insert(gFTestCoverageArgument);
            break;
    }

    auto argument_cxxflags = doim::gManager->obtainArgument(
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
    return doim::gManager->unique(compileCommand);
}

doim::SysCommandSPtr CxxCompiler::linkCommand(const doim::FsDirectorySPtr& directory,
                                              const doim::CxxProgramSPtr& program) const
{
    auto linkArguments = std::make_shared<doim::SysArgumentSet>();
    linkArguments->insert(gStdLibc11Argument);

    switch (program->purpose())
    {
        case doim::CxxProgram::EPurpose::kDebug:
            break;
        case doim::CxxProgram::EPurpose::kRelease:
            break;
        case doim::CxxProgram::EPurpose::kProfile:
            linkArguments->insert(gCoverageArgument);
            break;
    }

    for (const auto& cxxLibrary : *program->staticLibraries())
    {
        auto argument_L = doim::gManager->obtainArgument(
            "-L " + cxxLibrary->binary()->directory()->path(directory));
        linkArguments->insert(argument_L);

        std::string name = cxxLibrary->binary()->name();
        auto argument_l =
            doim::gManager->obtainArgument("-l" + name.substr(3, name.size() - 5));
        linkArguments->insert(argument_l);
    }

    for (const auto& objectFile : *program->cxxObjectFiles())
    {
        auto argument_obj =
            doim::gManager->obtainArgument(objectFile->file()->path(directory));
        linkArguments->insert(argument_obj);
    }

    const auto& path = program->file()->path(directory);
    auto argument_o = doim::gManager->obtainArgument("-o " + path);
    linkArguments->insert(argument_o);
    linkArguments = doim::gManager->unique(linkArguments);

    auto linkCommand = std::make_shared<doim::SysCommand>(mCompiler, linkArguments);
    return doim::gManager->unique(linkCommand);
}
}
