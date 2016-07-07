//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_cxx_framework.hpp"
#include "parser/dbs/e_depository.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_position.hpp"
#include "dom/cxx/cxx_library.h"

namespace parser
{
static auto r_cxxLibraryKw = r_str("cxx_library");
static auto r_cxxFileKw = r_str("cxx_file");
static auto r_protobufFileKw = r_str("protobuf_file");
static auto r_cxxHeaderKw = r_str("cxx_header");

typedef ObjectRef<dom::CxxLibrary> CxxLibraryRef;

struct CxxLibrarySet
{
    CxxLibrarySet(const doim::FsDirectorySPtr& location)
        : mCxxLibraryRef(location)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mCxxLibraries.clear(); });
    }

    auto insert()
    {
        return e_ref(
            [this](I& i1, I& i2) { mCxxLibraries.insert(mCxxLibraryRef.mObjectRef); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_cxxLibraryKw & r_ws & r_colon & r_empty() >> reset() &
               *(mCxxLibraryRef.rule(r_ws) >> insert()) & r_ws & r_semicolon;
    }

    CxxLibraryRef mCxxLibraryRef;
    dom::CxxLibrarySet mCxxLibraries;
};

struct CxxLibrary
{
    CxxLibrary(const doim::FsDirectorySPtr& location)
        : mDepositoryRef(location)
        , mFile(location)
        , mFiles(location)
        , mCxxLibraries(location)
        , mCxxLibraryRef(location)
    {
    }

    auto resetHeader()
    {
        return e_ref([this](I& i1, I& i2) {
            mVisibility.reset();
            mFiles.mFile.mDirectory.reset();
        });
    }

    auto attribute()
    {
        return e_ref([this](I& i1, I& i2) {
            if (mAttribute.mName == dom::CxxLibrary::gVisibility)
            {
                mVisibility = mAttribute.mAttribute;
            }
            else if (mAttribute.mName == dom::CxxLibrary::gDirectory)
            {
                mFiles.mFile.mDirectory.set(mAttribute.mValue->value());
            }
        });
    }

    static doim::CxxHeader::EVisibility visibility(const doim::AttributeSPtr& attribute)
    {
        if (attribute == nullptr || attribute->value() == dom::CxxLibrary::gPrivate)
            return doim::CxxHeader::EVisibility::kPrivate;
        if (attribute->value() == dom::CxxLibrary::gPublic)
            return doim::CxxHeader::EVisibility::kPublic;
        if (attribute->value() == dom::CxxLibrary::gProtected)
            return doim::CxxHeader::EVisibility::kProtected;
        ASSERT(false);
        return doim::CxxHeader::EVisibility::kProtected;
    }

    auto frameworks()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateFrameworkList(mFrameworks.mFrameworks);
        });
    }

    auto depository()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateDepository(mDepositoryRef.mObjectRef);
        });
    }

    auto cxxHeaders()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateCxxHeaders(visibility(mVisibility),
                                          mFiles.mFile.mDirectory.mDirectory,
                                          mFiles.mFiles);
        });
    }

    auto protobufs()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateProtobufsList(visibility(mVisibility),
                                             mFiles.mFile.mDirectory.mDirectory,
                                             mFiles.mFiles);
        });
    }

    auto libraries()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateCxxLibraries(mCxxLibraries.mCxxLibraries);
        });
    }

    auto files()
    {
        return e_ref(
            [this](I& i1, I& i2) { mCxxLibrary->updateCxxFilesList(mFiles.mFiles); });
    }

    auto binary()
    {
        return e_ref([this](I& i1, I& i2) { mCxxLibrary->updateBinary(mFile.mFile); });
    }

    auto name()
    {
        return e_ref([this](I& i1, I& i2) { mCxxLibrary = mCxxLibraryRef.mObjectRef; });
    }

    auto updateAttribute()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateAttribute(mAttribute.mAttribute);
        });
    }

    template <typename WS>
    auto r_cxxHeaderAttributes(const WS& r_ws)
    {
        return *(mAttribute.rule(r_ws) >> attribute());
    }

    template <typename WS>
    auto r_cxxHeader(const WS& r_ws)
    {
        return (r_ws & r_cxxHeaderKw >> resetHeader() & r_cxxHeaderAttributes(r_ws) &
                r_ws & r_colon & mFiles.rule(r_ws) & r_ws & r_semicolon) >>
               cxxHeaders();
    }

    template <typename WS>
    auto r_cxxFiles(const WS& r_ws)
    {
        return (r_ws & r_cxxFileKw & r_ws & r_colon & mFiles.mFile.mDirectory.r_reset() &
                mFiles.rule(r_ws) & r_ws & r_semicolon) >>
               files();
    }

    template <typename WS>
    auto r_frameworks(const WS& r_ws)
    {
        return r_ws & r_frameworkKw & r_ws & r_colon &
               mFrameworks.rule(r_ws) >> frameworks() & r_ws & r_semicolon;
    }

    template <typename WS>
    auto r_depository(const WS& r_ws)
    {
        return r_ws & r_depositoryKw & r_ws & r_colon &
               mDepositoryRef.rule(r_ws) >> depository() & r_ws & r_semicolon;
    }

    template <typename WS>
    auto r_cxxLibrary(const WS& r_ws)
    {
        return mCxxLibraries.rule(r_ws) >> libraries();
    }

    template <typename WS>
    auto r_protobufFile(const WS& r_ws)
    {
        return r_ws & r_protobufFileKw >> resetHeader() & r_cxxHeaderAttributes(r_ws) &
               r_ws & r_colon & mFiles.rule(r_ws) >> protobufs() & r_ws & r_semicolon;
    }

    template <typename WS>
    auto r_binary(const WS& r_ws)
    {
        return r_ws & r_binaryKw & r_ws & r_colon & mFile.mDirectory.r_reset() &
               mFile.rule(r_ws) >> binary() & r_ws & r_semicolon;
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_cxxLibraryKw & mCxxLibraryRef.rule(r_ws) >> name() &
               *(mAttribute.rule(r_ws) >> updateAttribute()) & r_ws & r_colon &
               *(r_frameworks(r_ws) | r_depository(r_ws) | r_cxxFiles(r_ws) |
                 r_protobufFile(r_ws) | r_cxxHeader(r_ws) | r_cxxLibrary(r_ws) |
                 r_binary(r_ws)) &
               r_ws & r_semicolon;
    }

    Attribute mAttribute;
    CxxFrameworkSet mFrameworks;
    DepositoryRef mDepositoryRef;
    File mFile;
    FileSet mFiles;
    CxxLibrarySet mCxxLibraries;
    CxxLibraryRef mCxxLibraryRef;

    doim::AttributeSPtr mVisibility;
    dom::CxxLibrarySPtr mCxxLibrary;
};
}
