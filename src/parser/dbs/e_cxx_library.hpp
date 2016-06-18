//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_file_set.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_position.hpp"
#include "dom/cxx/cxx_library.h"

namespace parser
{
static auto r_cxxLibraryKw = r_str("cxx_library");
static auto r_cxxFileKw = r_str("cxx_file");
static auto r_protobufFileKw = r_str("protobuf_file");
static auto r_cxxHeaderKw = r_str("cxx_header");

struct CxxLibraryRef
{
    CxxLibraryRef(Object& object)
        : mObject(object)
    {
    }

    void operator()(I& i1, I& i2)
    {
        mCxxLibrary = dom::CxxLibrary::obtain(mObject.mObject);
    }

    Object& mObject;
    dom::CxxLibrarySPtr mCxxLibrary;
};

struct CxxLibrarySet
{
    CxxLibrarySet(CxxLibraryRef& cxxLibraryRef)
        : mCxxLibraryRef(cxxLibraryRef)
    {
    }

    auto reset()
    {
        return e_ref([this](I& i1, I& i2) { mCxxLibraries.clear(); });
    }

    void operator()(I& i1, I& i2)
    {
        mCxxLibraries.insert(mCxxLibraryRef.mCxxLibrary);
    };

    CxxLibraryRef& mCxxLibraryRef;
    dom::CxxLibrarySet mCxxLibraries;
};

struct CxxLibrary
{
    CxxLibrary(Attribute& attribute,
               Directory& directory,
               File& file,
               FileSet& files,
               CxxLibrarySet& cxxLibraries,
               CxxLibraryRef& cxxLibraryRef)
        : mAttribute(attribute)
        , mDirectory(directory)
        , mFile(file)
        , mFiles(files)
        , mCxxLibraries(cxxLibraries)
        , mCxxLibraryRef(cxxLibraryRef)
    {
    }

    auto resetHeader()
    {
        return e_ref([this](I& i1, I& i2) {
            mVisibility.reset();
            mDirectory.reset();
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
                mDirectory.mDirectory =
                    doim::FsDirectory::obtain(mDirectory.mDefaultDirectory,
                                              mAttribute.mValue->value());
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

    auto cxxHeaders()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateCxxHeaders(visibility(mVisibility),
                                          mDirectory.mDirectory,
                                          mFiles.mFiles);
        });
    }

    auto protobufs()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateProtobufsList(visibility(mVisibility),
                                             mDirectory.mDirectory,
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
        return e_ref([this](I& i1, I& i2) { mCxxLibrary = mCxxLibraryRef.mCxxLibrary; });
    }

    auto updateAttribute()
    {
        return e_ref([this](I& i1, I& i2) {
            mCxxLibrary->updateAttribute(mAttribute.mAttribute);
        });
    }

    Attribute& mAttribute;
    Directory& mDirectory;
    File& mFile;
    FileSet& mFiles;
    CxxLibrarySet& mCxxLibraries;
    CxxLibraryRef& mCxxLibraryRef;

    doim::AttributeSPtr mVisibility;
    dom::CxxLibrarySPtr mCxxLibrary;
};
}
