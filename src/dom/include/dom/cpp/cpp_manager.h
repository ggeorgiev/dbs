//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cpp/cpp_library.hpp"

#include "doim/generic/object.hpp"

#include "im/initialization_manager.hpp"

#include <memory>
#include <unordered_map>
#include <utility>

namespace dom
{
class CppManager
{
public:
    typedef std::unordered_map<doim::ObjectSPtr, CppLibrarySPtr> LibraryMap;

    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    CppLibrarySPtr obtainCppLibrary(const doim::ObjectSPtr& object)
    {
        auto it = mCppLibraries.find(object);
        if (it == mCppLibraries.end())
        {
            auto value = LibraryMap::value_type(object, std::make_shared<CppLibrary>());
            it = mCppLibraries.insert(value).first;
        }
        return it->second;
    }

private:
    LibraryMap mCppLibraries;
};

typedef std::shared_ptr<CppManager> CppManagerSPtr;

extern CppManagerSPtr gCppManager;
}
