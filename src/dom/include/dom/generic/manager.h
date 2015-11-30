//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/fs/fs_manager.h"
#include "dom/generic/location.hpp"
#include "dom/generic/object.hpp"

#include "im/initialization_manager.hpp"

#include <stddef.h>
#include <functional>
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_set>

namespace dom
{
class Manager
{
public:
    static inline int initialization_rank()
    {
        return FsManager::initialization_rank() + im::InitializationManager::rank_step();
    }

    LocationSPtr obtainLocation(const LocationSPtr& base, const std::string& location)
    {
        return gFsManager->obtainDirectory(base, location.begin(), location.end());
    }

    LocationSPtr obtainLocation(const LocationSPtr& base,
                                const std::string::const_iterator& begin,
                                const std::string::const_iterator& end)
    {
        return gFsManager->obtainDirectory(base, begin, end);
    }

    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::Type type,
                            const std::string& object)
    {
        return obtainObject(base, type, object.begin(), object.end());
    }

    ObjectSPtr obtainObject(const LocationSPtr& base,
                            const Object::Type type,
                            const std::string::const_iterator& begin,
                            const std::string::const_iterator& end);

private:
    struct ObjectHasher
    {
        std::size_t operator()(const ObjectSPtr& object) const
        {
            return std::hash<int>()(static_cast<int>(object->type())) ^
                   std::hash<LocationSPtr>()(object->location()) ^
                   std::hash<std::string>()(object->name());
        }

        bool operator()(const ObjectSPtr& object1, const ObjectSPtr& object2) const
        {
            return object1->type() == object1->type() &&
                   object1->location() == object1->location() && object2->name() == object2->name();
        }
    };

    std::unordered_set<ObjectSPtr, ObjectHasher, ObjectHasher> mObjects;
};

typedef std::shared_ptr<Manager> ManagerSPtr;

extern ManagerSPtr gManager;
}
