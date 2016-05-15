//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_library.h"
#include "dom/generic/attribute.hpp"
#include "doim/set.hpp"
#include "err/err.h"
#include <functional>
#include <shared_ptr>
#include <str>
#include <unordered>
#include <vector>

namespace dom
{
CxxLibrary::CxxLibrary()
    : mType(EType::kUser)
    , mRecursiveCxxIncludeDirectoriesMemoization(
          std::make_shared<RecursiveCxxIncludeDirectoriesMemoization>())
{
    resetRecursiveCxxIncludeDirectoriesMemoization();
}

ECode CxxLibrary::updateAttribute(const Attribute& attribute)
{
    if (attribute.mName == "type")
    {
        if (attribute.mValue == "system")
            mType = EType::kSystem;
        else if (attribute.mValue == "test")
            mType = EType::kTest;
        else if (attribute.mValue == "3rdparty")
            mType = EType::kThirdParty;
        else if (attribute.mValue == "user")
            mType = EType::kUser;
        else
            EHBan(kUnable, attribute.mName, attribute.mValue);
    }
    else
    {
        EHBan(kUnable, attribute.mName);
    }
    EHEnd;
}

ECode CxxLibrary::updateBinary(const doim::FsFileSPtr& binary)
{
    mBinary = binary;
    EHEnd;
}

ECode CxxLibrary::updateCxxPublicHeaders(const doim::FsDirectorySPtr& directory,
                                         doim::FsFileSet& files)
{
    resetRecursiveCxxIncludeDirectoriesMemoization();

    mPublicHeadersDirectory = directory;
    mCxxPublicHeaders = doim::FsFileSet::make();
    mCxxPublicHeaders->swap(files);
    EHEnd;
}

ECode CxxLibrary::updateCxxLibraries(CxxLibrarySet& libraries)
{
    resetRecursiveCxxIncludeDirectoriesMemoization();

    mCxxLibraries.swap(libraries);
    EHEnd;
}

doim::CxxIncludeDirectory::EType CxxLibrary::cxxIncludeDirectoryType() const
{
    switch (mType)
    {
        case EType::kUser:
        case EType::kTest:
            return doim::CxxIncludeDirectory::EType::kUser;
        case EType::kThirdParty:
        case EType::kSystem:
            return doim::CxxIncludeDirectory::EType::kSystem;
    }
}

doim::CxxHeader::EType CxxLibrary::cxxHeaderType() const
{
    switch (mType)
    {
        case EType::kUser:
        case EType::kTest:
            return doim::CxxHeader::EType::kUser;
        case EType::kThirdParty:
        case EType::kSystem:
            return doim::CxxHeader::EType::kSystem;
    }
}

CxxLibrarySet CxxLibrary::recursiveCxxLibraries() const
{
    CxxLibrarySet libraries = mCxxLibraries;
    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libs = cxxLibrary->recursiveCxxLibraries();
        libraries.insert(libs.begin(), libs.end());
    }
    return libraries;
}

doim::CxxIncludeDirectorySPtr CxxLibrary::cxxPublicIncludeDirectory(
    const doim::FsDirectorySPtr& root) const
{
    if (publicHeadersDirectory() == nullptr)
        return nullptr;

    return doim::CxxIncludeDirectory::unique(cxxIncludeDirectoryType(),
                                             publicHeadersDirectory(),
                                             publicCxxHeaders(root));
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::sublibraryCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto directories = doim::CxxIncludeDirectorySet::make();

    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());
    }

    return doim::CxxIncludeDirectorySet::unique(directories);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::indirectCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto directories = doim::CxxIncludeDirectorySet::make();

    const auto& publicDirectory = cxxPublicIncludeDirectory(root);
    if (publicDirectory != nullptr)
        directories->insert(publicDirectory);

    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libDirectories = cxxLibrary->recursiveCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());
    }

    return doim::CxxIncludeDirectorySet::unique(directories);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::recursiveCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {
        auto directories = doim::CxxIncludeDirectorySet::make();

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

        return doim::CxxIncludeDirectorySet::unique(directories);
    };

    return mRecursiveCxxIncludeDirectoriesMemoization->get(mMemoizationHandle, root, fn);
}

doim::CxxHeaderSetSPtr CxxLibrary::publicCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();

    if (mCxxPublicHeaders != nullptr)
    {
        auto type = cxxHeaderType();
        const auto& directories = sublibraryCxxIncludeDirectories(root);
        for (const auto& header : *mCxxPublicHeaders)
        {
            auto cxxHeader = doim::CxxHeader::unique(type, header, directories);
            headers->insert(cxxHeader);
        }
    }
    return doim::CxxHeaderSet::unique(headers);
}

doim::CxxHeaderSetSPtr CxxLibrary::indirectCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();

    const auto& publicHeaders = publicCxxHeaders(root);
    if (publicHeaders != nullptr)
        headers->insert(publicHeaders->begin(), publicHeaders->end());

    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libHeaders = cxxLibrary->recursiveCxxHeaders(root);
        headers->insert(libHeaders->begin(), libHeaders->end());
    }
    return doim::CxxHeaderSet::unique(headers);
}

doim::CxxHeaderSetSPtr CxxLibrary::recursiveCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();

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
    return doim::CxxHeaderSet::unique(headers);
}

void CxxLibrary::resetRecursiveCxxIncludeDirectoriesMemoization()
{
    auto memorization = mRecursiveCxxIncludeDirectoriesMemoization;
    mMemoizationHandle = dp::Handle::create([memorization] { memorization->clear(); });
}
}
