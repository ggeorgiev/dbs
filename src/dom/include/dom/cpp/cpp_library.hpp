//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/attribute.hpp"

#include "dom/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <vector>
#include <unordered_set>

namespace dom
{
class CppLibrary
{
public:
    enum class Type
    {
        kSystem,
        kUser,
    };

    CppLibrary() : mType(Type::kUser) {}
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
        } else {
            EHBan(kUnable, attribute.mName);
        }
        EHEnd;
    }
    ECode updateBinary(const FsFileSPtr& binary)
    {
        mBinary = binary;
        EHEnd;
    }
    ECode updatePublicHeadersDirectory(const FsDirectorySPtr& publicHeaders)
    {
        mPublicHeaders = publicHeaders;
        EHEnd;
    }

    ECode updateCppFiles(std::unordered_set<dom::FsFileSPtr>& files)
    {
        mCppFiles.swap(files);
        EHEnd;
    }

    Type type() { return mType; }
    const std::unordered_set<dom::FsFileSPtr>& cppFiles() { return mCppFiles; }
    FsFileSPtr binary() { return mBinary; }
    FsDirectorySPtr publicHeadersDirectory() { return mPublicHeaders; }
private:
    Type mType;
    FsFileSPtr mBinary;
    FsDirectorySPtr mPublicHeaders;
    std::unordered_set<dom::FsFileSPtr> mCppFiles;
};

typedef std::shared_ptr<CppLibrary> CppLibrarySPtr;
}
