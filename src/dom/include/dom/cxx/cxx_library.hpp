//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/attribute.hpp"

#include "doim/manager.h"
#include "doim/cxx/cxx_object_file.hpp"
#include "doim/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <vector>
#include <unordered_set>

namespace dom
{
class CxxLibrary;
typedef std::shared_ptr<CxxLibrary> CxxLibrarySPtr;

class CxxLibrary
{
public:
    enum class Type
    {
        kSystem,
        kUser,
    };

    CxxLibrary()
        : mType(Type::kUser)
    {
    }

    // Updates
    ECode updateAttribute(const Attribute& attribute)
    {
        if (attribute.mName == "type")
        {
            if (attribute.mValue == "system")
                mType = Type::kSystem;
            else if (attribute.mValue == "user")
                mType = Type::kUser;
            else
                EHBan(kUnable, attribute.mName, attribute.mValue);
        }
        else
        {
            EHBan(kUnable, attribute.mName);
        }
        EHEnd;
    }
    ECode updateBinary(const doim::FsFileSPtr& binary)
    {
        mBinary = binary;
        EHEnd;
    }
    ECode updatePublicHeadersDirectory(const doim::FsDirectorySPtr& publicHeaders)
    {
        mPublicHeaders = publicHeaders;
        EHEnd;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
    {
        mCxxLibraries.swap(libraries);
        EHEnd;
    }

    ECode updateCxxFiles(std::unordered_set<doim::FsFileSPtr>& files)
    {
        mCxxFiles.swap(files);
        EHEnd;
    }

    // Accessors
    Type type()
    {
        return mType;
    }

    doim::FsFileSPtr binary()
    {
        return mBinary;
    }

    doim::FsDirectorySPtr publicHeadersDirectory() const
    {
        return mPublicHeaders;
    }

    const std::unordered_set<CxxLibrarySPtr>& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    const std::unordered_set<doim::FsFileSPtr>& cxxFiles() const
    {
        return mCxxFiles;
    }

    // Computations
    std::unordered_set<doim::CxxIncludeDirectorySPtr> cxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        std::unordered_set<doim::CxxIncludeDirectorySPtr> directories;

        switch (mType)
        {
            case Type::kUser:
                directories.insert(
                    doim::gManager->obtainCxxIncludeDirectory(doim::CxxIncludeDirectory::
                                                                  Type::kUser,
                                                              publicHeadersDirectory()));
                break;
            case Type::kSystem:
                directories.insert(
                    doim::gManager->obtainCxxIncludeDirectory(doim::CxxIncludeDirectory::
                                                                  Type::kSystem,
                                                              publicHeadersDirectory()));
                break;
        }

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->cxxIncludeDirectories(root);
            directories.insert(libDirectories.begin(), libDirectories.end());
        }

        return directories;
    }

    std::unordered_set<doim::CxxObjectFileSPtr> cxxObjectFiles(
        const doim::FsDirectorySPtr& root,
        const doim::FsDirectorySPtr& intermediate) const
    {
        std::unordered_set<doim::CxxObjectFileSPtr> cxxObjectFiles;

        const auto& directories = cxxIncludeDirectories(root);

        for (const auto& cxxFile : mCxxFiles)
        {
            const auto& directory =
                doim::gManager->obtainCorrespondingDirectory(cxxFile->directory(),
                                                             root,
                                                             intermediate);
            const auto& output =
                doim::gManager->obtainFile(directory, cxxFile->name() + ".o");

            auto objectFile =
                std::make_shared<doim::CxxObjectFile>(cxxFile, directories, output);
            objectFile = doim::gManager->unique(objectFile);

            cxxObjectFiles.insert(objectFile);
        }
        return cxxObjectFiles;
    }

private:
    Type mType;
    doim::FsFileSPtr mBinary;
    doim::FsDirectorySPtr mPublicHeaders;
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
    std::unordered_set<doim::FsFileSPtr> mCxxFiles;
};
}
