//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <vector>
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
        auto addedFiles = difference<dom::FsFileSPtr>(files, mCppFiles);
        auto deletedFiles = difference<dom::FsFileSPtr>(mCppFiles, files);
        mCppFiles.swap(files);
        EHEnd;
    }

    ECode dumpShell(const dom::FsDirectorySPtr& directory, std::string& dump)
    {
        std::stringstream stream;

        stream << "CLANGBIN=/Users/george/github/dbs/clang/bin\n"
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

        std::vector<std::string> files;
        for (auto cppFile : mCppFiles)
        {
            files.push_back(cppFile->path(directory));
        }
        std::sort(files.begin(), files.end());

        stream << "FILES=\"";
        for (auto file : files)
            stream << " " << file;
        stream << "\"\n";

        stream << "CXXFLAGS=\"$CXXFLAGS -isystemgtest/include\"\n"
               << "LIBRARIES=\"$LIBRARIES -Lgtest/lib\"\n"

               << "CXXFLAGS=\"$CXXFLAGS -isystemboost/include\"\n"
               << "LIBRARIES=\"$LIBRARIES -Lboost/lib\"\n"

               << "CXXFLAGS=\"$CXXFLAGS -isystemcppformat/include\"\n"
               << "LIBRARIES=\"$LIBRARIES -Lcppformat/lib\"\n"

               << "LIBRARIES=\"$LIBRARIES -lboost_system -lboost_chrono -lformat\"\n"

               << "DEFINES=\"-DDEBUG\" && OPTOMIZATION=\"-O0\"\n"

               << "mkdir -p build\n"
               << "PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS \\\n"
               << "    src/main.cpp $FILES \\\n"
               << "    -o build/main \\\n"
               << "    $DEFINES $LIBRARIES || exit 1\n"
               << "echo done.\n";

        dump = stream.str();
        EHEnd;
    }

private:
    std::unordered_set<dom::FsFileSPtr> mCppFiles;
};

typedef std::shared_ptr<CppProgram> CppProgramSPtr;
}
