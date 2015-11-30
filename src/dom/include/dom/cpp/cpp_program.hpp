//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cpp/cpp_library.hpp"
#include "dom/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
class CppProgram
{
public:
    template <typename T>
    std::vector<T> difference(const std::unordered_set<T>& a, const std::unordered_set<T>& b)
    {
        std::vector<T> result;
        std::copy_if(a.begin(), a.end(), back_inserter(result), [&b](T needle) {
            return b.find(needle) == b.end();
        });
        return result;
    }

    ECode updateCppFiles(std::unordered_set<dom::FsFileSPtr>& files)
    {
        mCppFiles.swap(files);
        EHEnd;
    }

    ECode updateCppLibraries(std::unordered_set<dom::CppLibrarySPtr>& libraries)
    {
        mCppLibraries.swap(libraries);
        EHEnd;
    }

    ECode dumpShell(const dom::FsDirectorySPtr& directory, std::string& dump)
    {
        std::stringstream stream;

        stream << "CLANGBIN=" << directory->path(nullptr) << "clang/bin\n"
               << "CLANG=clang++\n"

               << "CXXFLAGS=\"-std=c++11 -stdlib=libc++\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -isysroot "
                  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/"
                  "SDKs/MacOSX10.11.sdk\"\n"

               << "CXXFLAGS=\"$CXXFLAGS -Isrc/const/include\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -Isrc/im/include\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -Isrc/err/include\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -Isrc/dom/include\"\n"
               << "CXXFLAGS=\"$CXXFLAGS -Isrc/parser/include\"\n";

        std::set<std::string> files;
        for (auto cppFile : mCppFiles)
            files.emplace(cppFile->path(directory));

        stream << "FILES=\"";
        for (auto file : files)
            stream << " " << file;
        stream << "\"\n";

        std::set<std::string> isystems;
        for (auto cppLibrary : mCppLibraries)
            isystems.emplace(cppLibrary->publicHeadersDirectory()->path(directory));

        stream << "ISYSTEM=\"";
        for (auto isystem : isystems)
            stream << " -isystem " << isystem;
        stream << "\"\n";


        stream << "LIBRARIES=\"$LIBRARIES -Lboost/lib\"\n"
               << "LIBRARIES=\"$LIBRARIES -Lcppformat/lib\"\n"
               << "LIBRARIES=\"$LIBRARIES -lboost_system -lboost_chrono -lformat\"\n"

               << "DEFINES=\"-DDEBUG\" && OPTOMIZATION=\"-O0\"\n"

               << "mkdir -p build\n"
               << "PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $ISYSTEM $CXXFLAGS \\\n"
               << "    src/main.cpp $FILES \\\n"
               << "    -o build/main \\\n"
               << "    $DEFINES $LIBRARIES || exit 1\n"
               << "echo done.\n";

        dump = stream.str();
        EHEnd;
    }

private:
    std::unordered_set<dom::CppLibrarySPtr> mCppLibraries;
    std::unordered_set<dom::FsFileSPtr> mCppFiles;
};

typedef std::shared_ptr<CppProgram> CppProgramSPtr;
}
