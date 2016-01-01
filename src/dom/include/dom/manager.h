//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.hpp"
#include "doim/generic/object.hpp"
#include "im/initialization_manager.hpp"
#include <memory>
#include <unordered_map>

namespace dom
{
class Manager
{
public:
    typedef std::unordered_map<doim::ObjectSPtr, CxxLibrarySPtr> LibraryMap;

    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    CxxLibrarySPtr obtainCxxLibrary(const doim::ObjectSPtr& object)
    {
        return obtain(object, mCxxLibraries);
    }

private:
    template <typename Key, typename Value>
    std::shared_ptr<Value> obtain(
        const std::shared_ptr<Key>& key,
        std::unordered_map<std::shared_ptr<Key>, std::shared_ptr<Value>>& map)
    {
        auto it = map.find(key);
        if (it == map.end())
        {
            auto value = typename std::unordered_map<
                std::shared_ptr<Key>,
                std::shared_ptr<Value>>::value_type(key, std::make_shared<Value>());
            it = map.insert(value).first;
        }
        return it->second;
    }

    LibraryMap mCxxLibraries;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
