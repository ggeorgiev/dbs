//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "tool/cxx/cxx_compiler.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_static_library.h"
#include "doim/fs/fs_file.h"
#include "doim/sys/sys_argument.h"
#include "doim/sys/sys_command.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include <memory>
#include <str>

namespace tool
{
doim::SysArgumentSPtr CxxCompiler::gOptimizationLevel0Argument =
    doim::SysArgument::global("-O0", CxxCompiler::gOptimizationLevel0Argument);
doim::SysArgumentSPtr CxxCompiler::gOptimizationLevel3Argument =
    doim::SysArgument::global("-O3", CxxCompiler::gOptimizationLevel3Argument);
doim::SysArgumentSPtr CxxCompiler::gDebuggingInformationArgument =
    doim::SysArgument::global("-g", CxxCompiler::gDebuggingInformationArgument);
doim::SysArgumentSPtr CxxCompiler::gStdCpp11Argument =
    doim::SysArgument::global("-std=c++11", CxxCompiler::gStdCpp11Argument);
doim::SysArgumentSPtr CxxCompiler::gStdCpp14Argument =
    doim::SysArgument::global("-std=c++14", CxxCompiler::gStdCpp14Argument);
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
    auto arguments = doim::SysArgumentSet::make();

    for (const auto& includeDirectory : *includeDirectories)
    {
        string value;
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

        arguments->insert(doim::SysArgument::unique(value));
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
    ASSERT(objectFile->cxxFile() != nullptr);

    auto compileArguments =
        includeArguments(directory, objectFile->cxxFile()->cxxIncludeDirectories());
    compileArguments->insert(gStdCpp14Argument);

    switch (objectFile->purpose())
    {
        case doim::CxxObjectFile::EPurpose::kDebug:
            compileArguments->insert(gOptimizationLevel0Argument);
            compileArguments->insert(gDebuggingInformationArgument);
            break;
        case doim::CxxObjectFile::EPurpose::kRelease:
            compileArguments->insert(gOptimizationLevel3Argument);
            break;
        case doim::CxxObjectFile::EPurpose::kProfile:
            compileArguments->insert(gFProfileArcArgument);
            compileArguments->insert(gFTestCoverageArgument);
            break;
    }

    const string& file = objectFile->cxxFile()->file()->path(directory);

    auto argument_c = doim::SysArgument::unique("-c " + file);
    compileArguments->insert(argument_c);

    auto argument_o =
        doim::SysArgument::unique("-o " + objectFile->file()->path(directory));
    compileArguments->insert(argument_o);
    compileArguments = doim::SysArgumentSet::unique(compileArguments);

    return doim::SysCommand::unique(mCompiler, compileArguments);
}

doim::SysCommandSPtr CxxCompiler::linkCommand(const doim::FsDirectorySPtr& directory,
                                              const doim::CxxProgramSPtr& program) const
{
    auto linkArguments = doim::SysArgumentSet::make();
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
        auto argument_L = doim::SysArgument::unique(
            "-L " + cxxLibrary->binary()->directory()->path(directory));
        linkArguments->insert(argument_L);

        string name = cxxLibrary->binary()->name();
        auto argument_l =
            doim::SysArgument::unique("-l" + name.substr(3, name.size() - 5));
        linkArguments->insert(argument_l);
    }

    for (const auto& objectFile : *program->cxxObjectFiles())
    {
        auto argument_obj =
            doim::SysArgument::unique(objectFile->file()->path(directory));
        linkArguments->insert(argument_obj);
    }

    const auto& path = program->file()->path(directory);
    auto argument_o = doim::SysArgument::unique("-o " + path);
    linkArguments->insert(argument_o);
    linkArguments = doim::SysArgumentSet::unique(linkArguments);

    return doim::SysCommand::unique(mCompiler, linkArguments);
}
}
