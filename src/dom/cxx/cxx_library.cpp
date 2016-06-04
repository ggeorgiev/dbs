//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_library.h"
#include "doim/generic/attribute.h"
#include "doim/set.hpp"
#include "err/err.h"
#include <functional>
#include <shared_ptr>
#include <unordered>
#include <vector>

namespace dom
{
doim::AttributeNameSPtr CxxLibrary::gType =
    doim::AttributeName::global("type", CxxLibrary::gType);
doim::AttributeValueSPtr CxxLibrary::gSystem =
    doim::AttributeValue::global("system", CxxLibrary::gSystem);
doim::AttributeValueSPtr CxxLibrary::gTest =
    doim::AttributeValue::global("test", CxxLibrary::gTest);
doim::AttributeValueSPtr CxxLibrary::g3rdparty =
    doim::AttributeValue::global("3rdparty", CxxLibrary::g3rdparty);
doim::AttributeValueSPtr CxxLibrary::gUser =
    doim::AttributeValue::global("user", CxxLibrary::gUser);

doim::AttributeNameSPtr CxxLibrary::gDirectory =
    doim::AttributeName::global("directory", CxxLibrary::gDirectory);

doim::AttributeNameSPtr CxxLibrary::gVisibility =
    doim::AttributeName::global("visibility", CxxLibrary::gVisibility);
doim::AttributeValueSPtr CxxLibrary::gPublic =
    doim::AttributeValue::global("public", CxxLibrary::gPublic);
doim::AttributeValueSPtr CxxLibrary::gProtected =
    doim::AttributeValue::global("protected", CxxLibrary::gProtected);
doim::AttributeValueSPtr CxxLibrary::gPrivate =
    doim::AttributeValue::global("private", CxxLibrary::gPrivate);

CxxLibrary::CxxLibrary()
{
    mCIDMemoization.reset();
}

ECode CxxLibrary::updateAttribute(const doim::AttributeSPtr& attribute)
{
    if (attribute->name() == gType)
    {
        if (attribute->value() == gSystem)
            mType = EType::kSystem;
        else if (attribute->value() == gTest)
            mType = EType::kTest;
        else if (attribute->value() == g3rdparty)
            mType = EType::kThirdParty;
        else if (attribute->value() == gUser)
            mType = EType::kUser;
        else
            EHBan(kUnable, attribute->name()->name(), attribute->value()->value());
    }
    else
    {
        EHBan(kUnable, attribute->name()->name());
    }
    EHEnd;
}

ECode CxxLibrary::updateBinary(const doim::FsFileSPtr& binary)
{
    mBinary = binary;
    EHEnd;
}

ECode CxxLibrary::updateCxxLibraries(CxxLibrarySet& libraries)
{
    mCIDMemoization.reset();

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
    auto directory = headersDirectory(doim::CxxHeader::EVisibility::kPublic);

    if (directory == nullptr)
        return nullptr;

    return doim::CxxIncludeDirectory::unique(cxxIncludeDirectoryType(),
                                             directory,
                                             publicCxxHeaders(root));
}

doim::CxxIncludeDirectorySPtr CxxLibrary::cxxProtectedIncludeDirectory(
    const doim::FsDirectorySPtr& root) const
{
    auto directory = headersDirectory(doim::CxxHeader::EVisibility::kProtected);

    if (directory == nullptr)
        return nullptr;

    return doim::CxxIncludeDirectory::unique(cxxIncludeDirectoryType(),
                                             directory,
                                             protectedCxxHeaders(root));
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::indirectPublicCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {

        auto directories = doim::CxxIncludeDirectorySet::make();

        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libDirectories =
                cxxLibrary->recursivePublicCxxIncludeDirectories(root);
            directories->insert(libDirectories->begin(), libDirectories->end());
        }

        return doim::CxxIncludeDirectorySet::unique(directories);
    };

    return mCIDMemoization.mIndirectPublic->get(mCIDMemoization.mHandle, root, fn);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::recursiveProtectedCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto directories = doim::CxxIncludeDirectorySet::make();

    const auto& protectedDirectory = cxxProtectedIncludeDirectory(root);
    if (protectedDirectory != nullptr)
        directories->insert(protectedDirectory);

    const auto& libDirectories = indirectPublicCxxIncludeDirectories(root);
    directories->insert(libDirectories->begin(), libDirectories->end());

    return doim::CxxIncludeDirectorySet::unique(directories);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::recursivePublicCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {

        auto directories = doim::CxxIncludeDirectorySet::make();

        const auto& publicDirectory = cxxPublicIncludeDirectory(root);
        if (publicDirectory != nullptr)
            directories->insert(publicDirectory);

        const auto& libDirectories = indirectPublicCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());

        return doim::CxxIncludeDirectorySet::unique(directories);
    };

    return mCIDMemoization.mRecursivePublic->get(mCIDMemoization.mHandle, root, fn);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::visibleCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {

        auto directories = doim::CxxIncludeDirectorySet::make();

        const auto& privateDirectory = cxxPrivateIncludeDirectory(root);
        if (privateDirectory != nullptr)
            directories->insert(privateDirectory);

        const auto& protectedDirectory = cxxProtectedIncludeDirectory(root);
        if (protectedDirectory != nullptr)
            directories->insert(protectedDirectory);

        const auto& libDirectories = recursivePublicCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());

        return doim::CxxIncludeDirectorySet::unique(directories);
    };

    return mCIDMemoization.mVisible->get(mCIDMemoization.mHandle, root, fn);
}

doim::CxxHeaderSetSPtr CxxLibrary::publicCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();

    const auto files = headerFiles(doim::CxxHeader::EVisibility::kPublic);

    if (files == nullptr)
        return doim::CxxHeaderSet::unique(headers);

    auto type = cxxHeaderType();
    const auto& directories = recursiveProtectedCxxIncludeDirectories(root);
    for (const auto& header : *files)
    {
        auto cxxHeader = doim::CxxHeader::unique(
            type, doim::CxxHeader::EVisibility::kPublic, header, directories, nullptr);
        headers->insert(cxxHeader);
    }
    return doim::CxxHeaderSet::unique(headers);
}

doim::CxxHeaderSetSPtr CxxLibrary::protectedCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();

    const auto files = headerFiles(doim::CxxHeader::EVisibility::kProtected);

    if (files == nullptr)
        return doim::CxxHeaderSet::unique(headers);

    auto type = cxxHeaderType();
    const auto& directories = indirectPublicCxxIncludeDirectories(root);
    for (const auto& header : *files)
    {
        auto cxxHeader = doim::CxxHeader::unique(
            type, doim::CxxHeader::EVisibility::kProtected, header, directories, nullptr);
        headers->insert(cxxHeader);
    }
    return doim::CxxHeaderSet::unique(headers);
}

doim::CxxHeaderSetSPtr CxxLibrary::recursivePublicCxxHeaders(
    const doim::FsDirectorySPtr& root) const
{
    auto headers = doim::CxxHeaderSet::make();

    const auto& publicHeaders = publicCxxHeaders(root);
    if (publicHeaders != nullptr)
        headers->insert(publicHeaders->begin(), publicHeaders->end());

    for (const auto& cxxLibrary : mCxxLibraries)
    {
        const auto& libHeaders = cxxLibrary->recursivePublicCxxHeaders(root);
        headers->insert(libHeaders->begin(), libHeaders->end());
    }
    return doim::CxxHeaderSet::unique(headers);
}

void CxxLibrary::CxxIncludeDirectoriesMemoization::reset()
{
    auto indirect = mIndirectPublic;
    auto recursive = mRecursivePublic;
    auto visible = mVisible;
    mHandle = dp::Handle::create([indirect, recursive, visible] {
        indirect->clear();
        recursive->clear();
        visible->clear();
    });
}
}
