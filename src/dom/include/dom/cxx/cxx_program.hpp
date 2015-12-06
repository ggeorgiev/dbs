//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.hpp"

#include "doim/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
class CxxProgram
{
public:
    template <typename T>
    std::vector<T> difference(const std::unordered_set<T>& a,
                              const std::unordered_set<T>& b)
    {
        std::vector<T> result;
        std::copy_if(a.begin(), a.end(), back_inserter(result), [&b](T needle) {
            return b.find(needle) == b.end();
        });
        return result;
    }

    ECode updateCxxFiles(std::unordered_set<doim::FsFileSPtr>& files)
    {
        mCxxFiles.swap(files);
        EHEnd;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
    {
        mCxxLibraries.swap(libraries);
        EHEnd;
    }

    ECode dumpShell(const doim::FsDirectorySPtr& directory, std::string& dump)
    {
        std::stringstream stream;

        stream << "CLANGBIN=" << directory->path(nullptr) << "clang/bin\n"
               << "CLANG=clang++\n"

               << "CXXFLAGS=\"-std=c++11 -stdlib=libc++\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -isysroot "
                  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/"
                  "Developer/SDKs/MacOSX10.11.sdk\"\n";

        std::set<std::string> files;
        std::set<std::string> fileDirectories;

        std::set<std::string> ies;
        std::set<std::string> isystems;
        std::set<std::string> lib_directories;
        std::set<std::string> lib_binaries;
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& headerDir = cxxLibrary->publicHeadersDirectory()->path(directory);
            switch (cxxLibrary->type())
            {
                case dom::CxxLibrary::Type::kSystem:
                    isystems.emplace(headerDir);
                    break;
                case dom::CxxLibrary::Type::kUser:
                    ies.emplace(headerDir);
                    break;
            }
            if (cxxLibrary->binary() != nullptr)
            {
                lib_directories.emplace(
                    cxxLibrary->binary()->directory()->path(directory));
                std::string name = cxxLibrary->binary()->name();
                lib_binaries.emplace(name.substr(3, name.size() - 5));
            }
            else
            {
                for (const auto& cxxFile : cxxLibrary->cxxFiles())
                {
                    files.emplace(cxxFile->path(directory));
                    fileDirectories.emplace("build/" +
                                            cxxFile->directory()->path(directory));
                }
            }
        }

        for (const auto& cxxFile : mCxxFiles)
            files.emplace(cxxFile->path(directory));

        std::stringstream argumentStream;

        for (const auto& isystem : isystems)
            argumentStream << " -isystem " << isystem;

        for (const auto& i : ies)
            argumentStream << " -I " << i;

        argumentStream << " -DDEBUG -O0";

        auto arguments = argumentStream.str();

        for (const auto& dir : fileDirectories)
            stream << "mkdir -p " << dir << "\n";

        std::stringstream objFilesStream;
        for (const auto& file : files)
        {
            stream << "PATH=$CLANGBIN:$PATH $CLANG $CXXFLAGS -c " << file << " -o build/"
                   << file << ".o" << arguments << "\n";

            objFilesStream << " build/" << file << ".o";
        }

        for (const auto& lib_directory : lib_directories)
            objFilesStream << " -L" << lib_directory;

        for (const auto& lib_binary : lib_binaries)
            objFilesStream << " -l" << lib_binary;

        stream << "PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS \\\n"
               << "    " << objFilesStream.str() << "\\\n"
               << "    -o build/main \\\n"
               << "    $DEFINES $LIBRARIES || exit 1\n"
               << "echo done.\n";

        dump = stream.str();
        EHEnd;
    }

private:
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
    std::unordered_set<doim::FsFileSPtr> mCxxFiles;
};

typedef std::shared_ptr<CxxProgram> CxxProgramSPtr;
}
