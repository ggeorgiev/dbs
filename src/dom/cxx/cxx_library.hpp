//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_private_headers_mixin.hpp"
#include "dom/generic/attribute.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include <memory>
#include <unordered_set>
#include <vector>

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
        kTest,
        kThirdParty,
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
            else if (attribute.mValue == "test")
                mType = Type::kTest;
            else if (attribute.mValue == "3rdparty")
                mType = Type::kThirdParty;
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
        mCxxPublicHeaders = std::make_shared<doim::FsFileSet>();
        mCxxPublicHeaders->swap(files);
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

    const doim::FsFileSetSPtr publicHeaders() const
    {
        return mCxxPublicHeaders;
    }

    const std::unordered_set<CxxLibrarySPtr>& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    // Computations
    doim::CxxIncludeDirectory::Type cxxIncludeDirectoryType() const
    {
        switch (mType)
        {
            case Type::kUser:
            case Type::kTest:
                return doim::CxxIncludeDirectory::Type::kUser;
            case Type::kThirdParty:
            case Type::kSystem:
                return doim::CxxIncludeDirectory::Type::kSystem;
        }
    }

    doim::CxxHeader::Type cxxHeaderType() const
    {
        switch (mType)
        {
            case Type::kUser:
            case Type::kTest:
                return doim::CxxHeader::Type::kUser;
            case Type::kThirdParty:
            case Type::kSystem:
                return doim::CxxHeader::Type::kSystem;
        }
    }

    std::unordered_set<CxxLibrarySPtr> recursiveCxxLibraries() const
    {
        std::unordered_set<CxxLibrarySPtr> libraries = mCxxLibraries;
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libs = cxxLibrary->recursiveCxxLibraries();
            libraries.insert(libs.begin(), libs.end());
        }
        return libraries;
    }

    doim::CxxIncludeDirectorySPtr cxxPublicIncludeDirectory(
        const doim::FsDirectorySPtr& root) const
    {
        if (publicHeadersDirectory() == nullptr)
            return nullptr;

        const auto& directory =
            std::make_shared<doim::CxxIncludeDirectory>(cxxIncludeDirectoryType(),
                                                        publicHeadersDirectory(),
                                                        publicCxxHeaders(root));
        return doim::gManager->unique(directory);
    }

    doim::CxxIncludeDirectorySetSPtr sublibraryCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::gManager->unique(directories);
    }

    doim::CxxIncludeDirectorySetSPtr indirectCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        const auto& publicDirectory = cxxPublicIncludeDirectory(root);
        if (publicDirectory != nullptr)
            directories->insert(publicDirectory);

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::gManager->unique(directories);
    }

    doim::CxxIncludeDirectorySetSPtr recursiveCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        const auto& privateDirectory = cxxPrivateIncludeDirectory(root);
        if (privateDirectory != nullptr)
            directories->insert(privateDirectory);

        const auto& publicDirectory = cxxPublicIncludeDirectory(root);
        if (publicDirectory != nullptr)
            directories->insert(publicDirectory);

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::gManager->unique(directories);
    }

    doim::CxxHeaderSetSPtr publicCxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();

        if (mCxxPublicHeaders != nullptr)
        {
            auto type = cxxHeaderType();
            const auto& directories = sublibraryCxxIncludeDirectories(root);
            for (const auto& header : *mCxxPublicHeaders)
            {
                const auto& cxxHeader =
                    std::make_shared<doim::CxxHeader>(type, header, directories);
                headers->insert(doim::gManager->unique(cxxHeader));
            }
        }
        return doim::gManager->unique(headers);
    }

    doim::CxxHeaderSetSPtr indirectCxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();

        const auto& publicHeaders = publicCxxHeaders(root);
        if (publicHeaders != nullptr)
            headers->insert(publicHeaders->begin(), publicHeaders->end());

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libHeaders = cxxLibrary->recursiveCxxHeaders(root);
            headers->insert(libHeaders->begin(), libHeaders->end());
        }
        return doim::gManager->unique(headers);
    }

    doim::CxxHeaderSetSPtr recursiveCxxHeaders(const doim::FsDirectorySPtr& root) const
    {
        auto headers = std::make_shared<doim::CxxHeaderSet>();

        const auto& publicHeaders = publicCxxHeaders(root);
        if (publicHeaders != nullptr)
            headers->insert(publicHeaders->begin(), publicHeaders->end());

        const auto& privateHeaders = privateCxxHeaders(root);
        if (privateHeaders != nullptr)
            headers->insert(privateHeaders->begin(), privateHeaders->end());

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libHeaders = cxxLibrary->recursiveCxxHeaders(root);
            headers->insert(libHeaders->begin(), libHeaders->end());
        }
        return doim::gManager->unique(headers);
    }

private:
    Type mType;
    doim::FsFileSPtr mBinary;

    doim::FsDirectorySPtr mPublicHeadersDirectory;
    doim::FsFileSetSPtr mCxxPublicHeaders;

    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
