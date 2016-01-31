//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "task/cxx/cxx_file_crc_task.h"
#include "dom/cxx/cxx_program.hpp"
#include "doim/cxx/cxx_file.hpp"
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
    CxxCompiler(const doim::FsFileSPtr& binary)
        : mBinary(binary)
    {
    }

    std::string commandArg(const doim::FsDirectorySPtr& directory,
                           const doim::CxxIncludeDirectorySetSPtr& includeDirectories)
    {
        std::stringstream stream;
        for (const auto& includeDirectory : *includeDirectories)
        {
            switch (includeDirectory->type())
            {
                case doim::CxxIncludeDirectory::Type::kUser:
                    stream << " -I " << includeDirectory->directory()->path(directory);
                    break;
                case doim::CxxIncludeDirectory::Type::kSystem:
                    stream << " -isystem "
                           << includeDirectory->directory()->path(directory);
                    break;
            }
        }

        return stream.str();
    }

    ECode command(const doim::FsDirectorySPtr& directory,
                  const doim::CxxObjectFileSPtr& objectFile,
                  std::string& cmd)
    {
        cmd.clear();

        auto task = std::make_shared<task::CxxFileCrcTask>(objectFile->cxxFile());
        EHTest((*task)(), objectFile->file()->path());

        uint64_t crc;
        db::gDatabase->get(objectFile->cxxFile()->file(), crc);

        if (task->crc() == crc)
            EHEnd;

        db::gDatabase->put(objectFile->cxxFile()->file(), task->crc());

        std::stringstream stream;
        stream << "mkdir -p " << objectFile->file()->directory()->path(directory) << "\n";

        stream << mBinary->path(directory);
        stream << " $CXXFLAGS";

        stream << " -c " << objectFile->cxxFile()->file()->path(directory);
        stream << " -o " << objectFile->file()->path(directory);

        stream << commandArg(directory, objectFile->cxxFile()->cxxIncludeDirectories());

        stream << " || exit 1\n";
        cmd = stream.str();
        EHEnd;
    }

    ECode command(const doim::FsDirectorySPtr& directory,
                  const dom::CxxProgramSPtr& program,
                  std::string& cmd)
    {
        const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");

        std::stringstream stream;

        stream << "CXXFLAGS=\"-std=c++11 -stdlib=libc++\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -isysroot "
                  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/"
                  "Developer/SDKs/MacOSX10.11.sdk\"\n";

        std::set<std::string> files;

        std::set<std::string> lib_directories;
        std::set<std::string> lib_binaries;
        for (const auto& cxxLibrary : program->recursiveCxxLibraries())
        {
            if (cxxLibrary->binary() != nullptr)
            {
                lib_directories.insert(
                    cxxLibrary->binary()->directory()->path(directory));
                std::string name = cxxLibrary->binary()->name();
                lib_binaries.insert(name.substr(3, name.size() - 5));
            }
            else
            {
                const auto& objectFiles =
                    cxxLibrary->cxxObjectFiles(directory, intermediate);

                for (const auto& objectFile : objectFiles)
                {
                    files.insert(objectFile->cxxFile()->file()->path(directory));
                    std::string cmd;
                    EHTest(command(directory, objectFile, cmd));
                    stream << cmd;
                }
            }
        }

        const auto& objectFiles = program->cxxObjectFiles(directory, intermediate);
        for (const auto& objectFile : objectFiles)
        {
            files.insert(objectFile->cxxFile()->file()->path(directory));
            std::string cmd;
            EHTest(command(directory, objectFile, cmd));
            stream << cmd;
        }

        std::stringstream objFilesStream;
        for (const auto& file : files)
            objFilesStream << " build/" << file << ".o";

        for (const auto& lib_directory : lib_directories)
            objFilesStream << " -L" << lib_directory;

        for (const auto& lib_binary : lib_binaries)
            objFilesStream << " -l" << lib_binary;

        stream << mBinary->path(directory) << " $OPTOMIZATION $CXXFLAGS \\\n"
               << "    " << objFilesStream.str() << "\\\n"
               << "    -o build/" << program->name() << " || exit 1\n"
               << "echo done.\n";

        cmd = stream.str();
        EHEnd;
    }

private:
    doim::FsFileSPtr mBinary;
};
}
