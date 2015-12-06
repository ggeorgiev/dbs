//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/attribute.hpp"

#include "doim/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <vector>
#include <unordered_set>

namespace dom
{
class CxxLibrary
{
public:
    enum class Type
    {
        kSystem,
        kUser,
    };

    CxxLibrary()
        : mType(Type::kUser)
    {
    }
    ECode updateAttribute(const Attribute& attribute)
    {
        if (attribute.mName == "type")
        {
            if (attribute.mValue == "system")
                mType = Type::kSystem;
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
    ECode updatePublicHeadersDirectory(const doim::FsDirectorySPtr& publicHeaders)
    {
        mPublicHeaders = publicHeaders;
        EHEnd;
    }

    ECode updateCxxFiles(std::unordered_set<doim::FsFileSPtr>& files)
    {
        mCxxFiles.swap(files);
        EHEnd;
    }

    Type type()
    {
        return mType;
    }
    const std::unordered_set<doim::FsFileSPtr>& cxxFiles()
    {
        return mCxxFiles;
    }
    doim::FsFileSPtr binary()
    {
        return mBinary;
    }
    doim::FsDirectorySPtr publicHeadersDirectory()
    {
        return mPublicHeaders;
    }

private:
    Type mType;
    doim::FsFileSPtr mBinary;
    doim::FsDirectorySPtr mPublicHeaders;
    std::unordered_set<doim::FsFileSPtr> mCxxFiles;
};

typedef std::shared_ptr<CxxLibrary> CxxLibrarySPtr;
}
