//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/cxx/cxx_library.h"
#include "doim/generic/attribute.h"
#include "doim/set.hpp"
#include "err/err.h"
#include <functional>
#include <shared_ptr>
#include <string>
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
    mCLMemoization.reset();
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
    mCLMemoization.reset();
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
    auto fn = [this](std::vector<dp::Handle::ControllerSPtr>& dependencies) {
        CxxLibrarySet libraries = mCxxLibraries;
        for (const auto& cxxLibrary : mCxxLibraries)
        {
            const auto& libs = cxxLibrary->recursiveCxxLibraries();
            libraries.insert(libs.begin(), libs.end());
        }
        return libraries;
    };

    return mCLMemoization.mRecursive->get(mCLMemoization.mHandle, fn);
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
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {

        auto directories = doim::CxxIncludeDirectorySet::make();

        const auto& protectedDirectories =
            cxxIncludeDirectories(doim::CxxHeader::EVisibility::kProtected, root);
        directories->insert(protectedDirectories.begin(), protectedDirectories.end());

        const auto& libDirectories = indirectPublicCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());

        return doim::CxxIncludeDirectorySet::unique(directories);
    };

    return mCIDMemoization.mRecursiveProtected->get(mCIDMemoization.mHandle, root, fn);
}

doim::CxxIncludeDirectorySetSPtr CxxLibrary::recursivePublicCxxIncludeDirectories(
    const doim::FsDirectorySPtr& root) const
{
    auto fn = [this](doim::FsDirectorySPtr root,
                     std::vector<dp::Handle::ControllerSPtr>& dependencies) {

        auto directories = doim::CxxIncludeDirectorySet::make();

        const auto& publicDirectories =
            cxxIncludeDirectories(doim::CxxHeader::EVisibility::kPublic, root);
        directories->insert(publicDirectories.begin(), publicDirectories.end());

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

        const auto& privateDirectories =
            cxxIncludeDirectories(doim::CxxHeader::EVisibility::kPrivate, root);
        directories->insert(privateDirectories.begin(), privateDirectories.end());

        const auto& protectedDirectories =
            cxxIncludeDirectories(doim::CxxHeader::EVisibility::kProtected, root);
        directories->insert(protectedDirectories.begin(), protectedDirectories.end());

        const auto& libDirectories = recursivePublicCxxIncludeDirectories(root);
        directories->insert(libDirectories->begin(), libDirectories->end());

        return doim::CxxIncludeDirectorySet::unique(directories);
    };

    return mCIDMemoization.mVisible->get(mCIDMemoization.mHandle, root, fn);
}

void CxxLibrary::CxxLibrariesMemoization::reset()
{
    auto recursive = mRecursive;
    mHandle = dp::Handle::create([recursive] { recursive->clear(); });
}

void CxxLibrary::CxxIncludeDirectoriesMemoization::reset()
{
    auto indirect = mIndirectPublic;
    auto rPublic = mRecursivePublic;
    auto rProtected = mRecursiveProtected;
    auto visible = mVisible;
    mHandle = dp::Handle::create([indirect, rPublic, rProtected, visible] {
        indirect->clear();
        rPublic->clear();
        rProtected->clear();
        visible->clear();
    });
}
}
