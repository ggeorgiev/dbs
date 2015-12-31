//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/attribute.hpp"
#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_private_headers_mixin.hpp"

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

class CxxLibrary : public CxxFilesMixin<CxxLibrary>,
                   public CxxPrivateHeadersMixin<CxxLibrary>
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

    ECode updateCxxPublicHeaders(const doim::FsDirectorySPtr& directory,
                                 std::unordered_set<doim::FsFileSPtr>& files)
    {
        mPublicHeadersDirectory = directory;
        mCxxPublicHeadersList.swap(files);
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
        return mPublicHeadersDirectory;
    }

    const std::unordered_set<doim::FsFileSPtr>& publicHeaders() const
    {
        return mCxxPublicHeadersList;
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

    doim::CxxHeaderSetSPtr cxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();

        return doim::gManager->unique(headers);
    }

private:
    Type mType;
    doim::FsFileSPtr mBinary;

    // TODO: So far we assume that all headers are coming from a single public directory
    //       Obviously this is not going to be true for all components.
    //       We should have this computed based on the header files, that will give us
    //       the ability to have optionally move all public headers in a intermediate
    //       directory.
    doim::FsDirectorySPtr mPublicHeadersDirectory;
    std::unordered_set<doim::FsFileSPtr> mCxxPublicHeadersList;

    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
