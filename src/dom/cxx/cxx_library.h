//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_headers_mixin.hpp"
#include "dom/cxx/protobufs_mixin.hpp"
#include "dom/element.hpp"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "dp/handle.hpp"
#include "dp/map_container.hpp"
#include "dp/memoization.hpp"
#include "dp/solitary_container.hpp"
#include "err/err.h"
#include <shared_ptr>
#include <unordered>

namespace dom
{
class CxxLibrary;
typedef shared_ptr<CxxLibrary> CxxLibrarySPtr;
typedef unordered_set<CxxLibrarySPtr> CxxLibrarySet;
typedef shared_ptr<CxxLibrarySet> CxxLibrarySetSPtr;

class CxxLibrary : public Element<CxxLibrary>,
                   public ProtobufsMixin<CxxLibrary>,
                   public CxxFilesMixin<CxxLibrary>,
                   public CxxHeadersMixin<CxxLibrary>
{
public:
    static doim::AttributeNameSPtr gType;
    static doim::AttributeValueSPtr gSystem;
    static doim::AttributeValueSPtr gTest;
    static doim::AttributeValueSPtr g3rdparty;
    static doim::AttributeValueSPtr gUser;

    static doim::AttributeNameSPtr gDirectory;

    static doim::AttributeNameSPtr gVisibility;
    static doim::AttributeValueSPtr gPublic;
    static doim::AttributeValueSPtr gProtected;
    static doim::AttributeValueSPtr gPrivate;

    enum class EType
    {
        kSystem,
        kTest,
        kThirdParty,
        kUser,
    };

    CxxLibrary();

    // Accessors
    EType type()
    {
        return mType;
    }

    doim::FsFileSPtr binary()
    {
        return mBinary;
    }

    const CxxLibrarySet& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    // Updates
    ECode updateAttribute(const doim::AttributeSPtr& attribute);

    ECode updateBinary(const doim::FsFileSPtr& binary);

    ECode updateCxxLibraries(CxxLibrarySet& libraries);

    // Computations
    doim::CxxIncludeDirectory::EType cxxIncludeDirectoryType() const;

    doim::CxxHeader::EType cxxHeaderType() const;

    CxxLibrarySet recursiveCxxLibraries() const;

    // Returns indirect include directories that can be used from objects of this library
    doim::CxxIncludeDirectorySetSPtr indirectPublicCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    // Returns the protected and indirect include directories that can be used from
    // public headers of this library.
    doim::CxxIncludeDirectorySetSPtr recursiveProtectedCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    // Returns direct and indirect include directories that can be used from objects
    // that depend on this library
    doim::CxxIncludeDirectorySetSPtr recursivePublicCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    // Returns direct and indirect include directories that objects from this library are
    // able to see. This includes private and all depending public.
    doim::CxxIncludeDirectorySetSPtr visibleCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

private:
    EType mType = EType::kUser;
    doim::FsFileSPtr mBinary;

    CxxLibrarySet mCxxLibraries;

    struct CxxLibrariesMemoization
    {
        typedef dp::Memoization<dp::SolitaryContainer, dom::CxxLibrarySet> Memoization;
        typedef shared_ptr<Memoization> MemoizationSPtr;

        void reset();

        dp::HandleSPtr mHandle;
        MemoizationSPtr mRecursive = Memoization::make();
    } mCLMemoization;

    struct CxxIncludeDirectoriesMemoization
    {
        typedef dp::Memoization<dp::MapContainer,
                                doim::CxxIncludeDirectorySetSPtr,
                                doim::FsDirectorySPtr>
            Memoization;
        typedef shared_ptr<Memoization> MemoizationSPtr;

        void reset();

        dp::HandleSPtr mHandle;
        MemoizationSPtr mIndirectPublic = Memoization::make();
        MemoizationSPtr mRecursivePublic = Memoization::make();
        MemoizationSPtr mRecursiveProtected = Memoization::make();
        MemoizationSPtr mVisible = Memoization::make();
    } mCIDMemoization;
};
}
