//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_library.hpp"
#include "dom/cxx/cxx_program.hpp"
#include "doim/generic/object.h"
#include "im/initialization_manager.hpp"
#include <memory>
#include <unordered_map>

namespace dom
{
template <typename K, typename V>
class ManagerMixin
{
public:
    typedef K Key;
    typedef std::shared_ptr<Key> KeySPtr;

    typedef V Value;
    typedef std::shared_ptr<Value> ValueSPtr;

    typedef std::unordered_map<KeySPtr, ValueSPtr> Map;

    ValueSPtr obtain(const std::shared_ptr<Key>& key)
    {
        auto it = mMixinObjects.find(key);
        if (it == mMixinObjects.end())
        {
            auto value = typename Map::value_type(key, std::make_shared<Value>());
            it = mMixinObjects.insert(value).first;
        }
        return it->second;
    }

protected:
    Map mMixinObjects;
};

class Manager : public ManagerMixin<doim::Object, dom::CxxLibrary>,
                public ManagerMixin<doim::Object, dom::CxxProgram>
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    CxxLibrarySPtr obtainCxxLibrary(const doim::ObjectSPtr& object)
    {
        return ManagerMixin<doim::Object, dom::CxxLibrary>::obtain(object);
    }

    CxxProgramSPtr obtainCxxProgram(const doim::ObjectSPtr& object)
    {
        return ManagerMixin<doim::Object, dom::CxxProgram>::obtain(object);
    }
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
