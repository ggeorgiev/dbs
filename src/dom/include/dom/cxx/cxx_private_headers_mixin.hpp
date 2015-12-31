//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.hpp"

#include "doim/manager.h"
#include "doim/cxx/cxx_file.hpp"
#include "doim/cxx/cxx_object_file.hpp"
#include "doim/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class CxxPrivateHeadersMixin
{
public:
    typedef T Subject;

    ECode updateCxxPrivateHeadersList(std::unordered_set<doim::FsFileSPtr>& files)
    {
        mCxxPrivateHeadersList.swap(files);
        EHEnd;
    }

    const std::unordered_set<doim::FsFileSPtr>& cxxPrivateHeadersList() const
    {
        return mCxxPrivateHeadersList;
    }

    std::unordered_set<doim::CxxHeaderSPtr> cxxPrivateHeaders(
        const doim::FsDirectorySPtr& root) const
    {
        std::unordered_set<doim::CxxHeaderSPtr> cxxHeaders;

        const auto& directories =
            static_cast<const Subject*>(this)->cxxIncludeDirectories(root);

        for (const auto& fsFile : mCxxPrivateHeadersList)
        {
            auto cxxHeader = std::make_shared<doim::CxxHeader>(fsFile, directories);
            cxxHeaders.insert(doim::gManager->unique(cxxHeader));
        }

        return cxxHeaders;
    }

private:
    std::unordered_set<doim::FsFileSPtr> mCxxPrivateHeadersList;
};
}
