//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_private_headers_mixin.hpp"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "dp/handle.hpp"
#include "dp/map_container.hpp"
#include "dp/memoization.hpp"
#include "err/err.h"
#include <shared_ptr>
#include <unordered_set>

namespace dom
{
struct Attribute;
} // namespace dom

namespace dom
{
class CxxLibrary;
typedef shared_ptr<CxxLibrary> CxxLibrarySPtr;
typedef unordered_set<CxxLibrarySPtr> CxxLibrarySet;
typedef shared_ptr<CxxLibrarySet> CxxLibrarySetSPtr;

class CxxLibrary : public enable_make_shared<CxxLibrary>,
                   public CxxFilesMixin<CxxLibrary>,
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

    CxxLibrary();

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

    const CxxLibrarySet& cxxLibraries() const
    {
        return mCxxLibraries;
    }

    // Updates
    ECode updateAttribute(const Attribute& attribute);

    ECode updateBinary(const doim::FsFileSPtr& binary);

    ECode updateCxxPublicHeaders(const doim::FsDirectorySPtr& directory,
                                 doim::FsFileSet& files);

    ECode updateCxxLibraries(CxxLibrarySet& libraries);

    // Computations
    doim::CxxIncludeDirectory::EType cxxIncludeDirectoryType() const;

    doim::CxxHeader::Type cxxHeaderType() const;

    CxxLibrarySet recursiveCxxLibraries() const;

    doim::CxxIncludeDirectorySPtr cxxPublicIncludeDirectory(
        const doim::FsDirectorySPtr& root) const;

    doim::CxxIncludeDirectorySetSPtr sublibraryCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    doim::CxxIncludeDirectorySetSPtr indirectCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    doim::CxxIncludeDirectorySetSPtr recursiveCxxIncludeDirectories(
        const doim::FsDirectorySPtr& root) const;

    doim::CxxHeaderSetSPtr publicCxxHeaders(const doim::FsDirectorySPtr& root) const;

    doim::CxxHeaderSetSPtr indirectCxxHeaders(const doim::FsDirectorySPtr& root) const;

    doim::CxxHeaderSetSPtr recursiveCxxHeaders(const doim::FsDirectorySPtr& root) const;

private:
    Type mType;
    doim::FsFileSPtr mBinary;

    doim::FsDirectorySPtr mPublicHeadersDirectory;
    doim::FsFileSetSPtr mCxxPublicHeaders;

    CxxLibrarySet mCxxLibraries;

    void resetRecursiveCxxIncludeDirectoriesMemoization();

    typedef dp::Memoization<dp::MapContainer,
                            doim::CxxIncludeDirectorySetSPtr,
                            doim::FsDirectorySPtr>
        RecursiveCxxIncludeDirectoriesMemoization;
    typedef shared_ptr<RecursiveCxxIncludeDirectoriesMemoization>
        RecursiveCxxIncludeDirectoriesMemoizationSPtr;

    dp::HandleSPtr mMemoizationHandle;
    RecursiveCxxIncludeDirectoriesMemoizationSPtr
        mRecursiveCxxIncludeDirectoriesMemoization;
};
}
