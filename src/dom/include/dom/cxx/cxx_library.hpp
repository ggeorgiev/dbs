//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_private_headers_mixin.hpp"
#include "dom/generic/attribute.hpp"
#include "doim/cxx/cxx_file.hpp"
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

    std::unordered_set<CxxLibrarySPtr> allCxxLibraries()
    {
        std::unordered_set<CxxLibrarySPtr> libraries = mCxxLibraries;
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libs = cxxLibrary->allCxxLibraries();
            libraries.insert(libs.begin(), libs.end());
        }
        return libraries;
    }

    // Computations
    doim::CxxIncludeDirectorySetSPtr cxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const
    {
        auto directories = std::make_shared<doim::CxxIncludeDirectorySet>();

        if (publicHeadersDirectory() != nullptr)
        {
            doim::CxxIncludeDirectory::Type type;
            switch (mType)
            {
                case Type::kUser:
                case Type::kTest:
                    type = doim::CxxIncludeDirectory::Type::kUser;
                    break;
                case Type::kThirdParty:
                case Type::kSystem:
                    type = doim::CxxIncludeDirectory::Type::kSystem;
                    break;
            }

            const auto& directory =
                std::make_shared<doim::CxxIncludeDirectory>(type,
                                                            publicHeadersDirectory(),
                                                            publicHeaders());
            directories->insert(doim::gManager->unique(directory));
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

        doim::CxxHeader::Type type;
        switch (mType)
        {
            case Type::kUser:
            case Type::kTest:
                type = doim::CxxHeader::Type::kUser;
                break;
            case Type::kThirdParty:
            case Type::kSystem:
                type = doim::CxxHeader::Type::kSystem;
                break;
        }

        if (mCxxPublicHeaders != nullptr)
        {
            const auto& directories = cxxIncludeDirectories(root);
            for (const auto& header : *mCxxPublicHeaders)
            {
                const auto& cxxHeader =
                    std::make_shared<doim::CxxHeader>(type, header, directories);
                headers->insert(doim::gManager->unique(cxxHeader));
            }
        }
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libHeaders = cxxLibrary->cxxHeaders(root);
            headers->insert(libHeaders->begin(), libHeaders->end());
        }
        return doim::gManager->unique(headers);
    }

private:
    Type mType;
    doim::FsFileSPtr mBinary;

    // TODO: So far we assume that all headers are coming from a single public
    // directory
    //       Obviously this is not going to be true for all components.
    //       We should have this computed based on the header files, that will give us
    //       the ability to have optionally move all public headers in a intermediate
    //       directory.
    doim::FsDirectorySPtr mPublicHeadersDirectory;
    doim::FsFileSetSPtr mCxxPublicHeaders;

    std::unordered_set<CxxLibrarySPtr> mCxxLibraries;
};
}
