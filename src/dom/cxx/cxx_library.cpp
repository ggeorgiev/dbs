//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_library.h"
#include "dom/generic/attribute.hpp"
#include "doim/manager.h"
#include "err/err.h"
#include <functional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace dom
{
CxxLibrary::CxxLibrary()
    : mType(Type::kUser)
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

ECode CxxLibrary::updateBinary(const doim::FsFileSPtr& binary)
{
    mBinary = binary;
    EHEnd;
}

ECode CxxLibrary::updateCxxPublicHeaders(const doim::FsDirectorySPtr& directory,
                                         std::unordered_set<doim::FsFileSPtr>& files)
{
    resetRecursiveCxxIncludeDirectoriesMemoization();

    mPublicHeadersDirectory = directory;
    mCxxPublicHeaders = std::make_shared<doim::FsFileSet>();
    mCxxPublicHeaders->swap(files);
    EHEnd;
}

ECode CxxLibrary::updateCxxLibraries(std::unordered_set<CxxLibrarySPtr>& libraries)
{
    resetRecursiveCxxIncludeDirectoriesMemoization();

    mCxxLibraries.swap(libraries);
    EHEnd;
}

doim::CxxIncludeDirectory::EType CxxLibrary::cxxIncludeDirectoryType() const
{
    switch (mType)
    {
        case Type::kUser:
        case Type::kTest:
            return doim::CxxIncludeDirectory::EType::kUser;
        case Type::kThirdParty:
        case Type::kSystem:
            return doim::CxxIncludeDirectory::EType::kSystem;
    }
}

doim::CxxHeader::Type CxxLibrary::cxxHeaderType() const
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

std::unordered_set<CxxLibrarySPtr> CxxLibrary::recursiveCxxLibraries() const
{
    std::unordered_set<CxxLibrarySPtr> libraries = mCxxLibraries;
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

    const auto& directory = doim::CxxIncludeDirectory::make(cxxIncludeDirectoryType(),
                                                            publicHeadersDirectory(),
                                                            publicCxxHeaders(root));
    return doim::gManager->unique(directory);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::sublibraryCxxIncludeDirectories(
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

doim::CxxIncludeDirectorySetSPtr CxxLibrary::indirectCxxIncludeDirectories(
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

doim::CxxIncludeDirectorySetSPtr CxxLibrary::recursiveCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {
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
    };

    return mRecursiveCxxIncludeDirectoriesMemoization->get(mMemoizationHandle, root, fn);
}

doim::CxxHeaderSetSPtr CxxLibrary::publicCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = std::make_shared<doim::CxxHeaderSet>();

    if (mCxxPublicHeaders != nullptr)
    {
        auto type = cxxHeaderType();
        const auto& directories = sublibraryCxxIncludeDirectories(root);
        for (const auto& header : *mCxxPublicHeaders)
        {
            const auto& cxxHeader = doim::CxxHeader::make(type, header, directories);
            headers->insert(doim::gManager->unique(cxxHeader));
        }
    }
    return doim::gManager->unique(headers);
}

doim::CxxHeaderSetSPtr CxxLibrary::indirectCxxHeaders(
    const doim::FsDirectorySPtr& root) const
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

doim::CxxHeaderSetSPtr CxxLibrary::recursiveCxxHeaders(
    const doim::FsDirectorySPtr& root) const
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

void CxxLibrary::resetRecursiveCxxIncludeDirectoriesMemoization()
{
    auto memorization = mRecursiveCxxIncludeDirectoriesMemoization;
    mMemoizationHandle = dp::Handle::create([memorization] { memorization->clear(); });
}
}
