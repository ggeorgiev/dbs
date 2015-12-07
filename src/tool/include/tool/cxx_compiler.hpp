//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_object_file.hpp"

#include "dom/cxx/cxx_program.hpp"

#include <memory>
#include <string>
#include <sstream>
#include <set>

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

    std::string commandArg(
        const doim::FsDirectorySPtr& directory,
        const std::unordered_set<doim::CxxIncludeDirectorySPtr>& includeDirectories)
    {
        std::stringstream stream;
        for (const auto& includeDirectory : includeDirectories)
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

    std::string command(const doim::FsDirectorySPtr& directory,
                        const doim::CxxObjectFileSPtr& objectFile)
    {
        std::stringstream stream;
        stream << "mkdir -p " << objectFile->outputFile()->directory()->path(directory)
               << "\n";

        stream << mBinary->path(directory);
        stream << " $CXXFLAGS";

        stream << " -c " << objectFile->cxxFile()->path(directory);
        stream << " -o " << objectFile->outputFile()->path(directory);

        stream << commandArg(directory, objectFile->cxxIncludeDirectories());
        return stream.str();
    }

    std::string command(const doim::FsDirectorySPtr& directory,
                        const dom::CxxProgramSPtr& program)
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
        for (const auto& cxxLibrary : program->cxxLibraries())
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
                    files.insert(objectFile->cxxFile()->path(directory));
                    stream << command(directory, objectFile) << " || exit 1\n";
                }
            }
        }

        const auto& objectFiles = program->cxxObjectFiles(directory, intermediate);
        for (const auto& objectFile : objectFiles)
        {
            files.insert(objectFile->cxxFile()->path(directory));
            stream << command(directory, objectFile) << " || exit 1\n";
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
               << "    -o build/main \\\n"
               << "    $DEFINES $LIBRARIES || exit 1\n"
               << "echo done.\n";

        return stream.str();
    }

private:
    doim::FsFileSPtr mBinary;
};
}
