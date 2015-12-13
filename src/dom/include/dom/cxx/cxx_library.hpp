//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/attribute.hpp"
#include "dom/cxx/cxx_files_mixin.hpp"

#include "doim/manager.h"
#include "doim/cxx/cxx_file.hpp"
#include "doim/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <vector>
#include <unordered_set>

namespace dom
{
class CxxLibrary;
typedef std::shared_ptr<CxxLibrary> CxxLibrarySPtr;

class CxxLibrary : public CxxFilesMixin<CxxLibrary>
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
    ECode updatePublicHeadersDirectory(

        const doim::FsDirectorySPtr& publicHeaders)
    {
        mPublicHeaders = publicHeaders;
        EHEnd;
    }

    ECode updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
    {
        mCxxLibraries.swap(libraries);
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

    // Computations
    doim::CxxIncludeDirectorySetSPtr cxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        switch (mType)
        {
            case Type::kUser:
                directories->insert(
                    doim::gManager->obtainCxxIncludeDirectory(doim::CxxIncludeDirectory::
                                                                  Type::kUser,
                                                              publicHeadersDirectory()));
                break;
            case Type::kSystem:
                directories->insert(
                    doim::gManager->obtainCxxIncludeDirectory(doim::CxxIncludeDirectory::
                                                                  Type::kSystem,
                                                              publicHeadersDirectory()));
                break;
        }

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->cxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::gManager->unique(directories);
    }

private:
    Type mType;
    doim::FsFileSPtr mBinary;
    doim::FsDirectorySPtr mPublicHeaders;
    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
