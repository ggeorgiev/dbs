//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/generic/location.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>

namespace doim
{
/*
 * Generic Object
 *
 * Generic objects are identified by name and location. For sake of simplicity and code
 * efficiency
 * the location of the object is the shared with the fs directory infrastructure.
 *
 */

class Object;
typedef std::shared_ptr<Object> ObjectSPtr;

class Object
{
public:
    enum class Type
    {
        kInvalid = 0,
        kCxxLibrary,
        kCxxProgram,
    };

    Object(const Type type, const std::string& name, const LocationSPtr& location)
        : mType(type)
        , mName(name)
        , mLocation(location)
    {
    }

    Type type()
    {
        return mType;
    }
    const std::string& name() const
    {
        return mName;
    }
    const LocationSPtr& location() const
    {
        return mLocation;
    }

    struct Hasher
    {
        std::size_t operator()(const ObjectSPtr& object) const
        {
            auto seed = std::hash<int>()(static_cast<int>(object->type()));
            boost::hash_combine(seed, hashL(object->location()));
            boost::hash_combine(seed, hashString(object->name()));
            return seed;
        }

        bool operator()(const ObjectSPtr& object1, const ObjectSPtr& object2) const
        {
            return object1->type() == object1->type() &&
                   object1->location() == object1->location() &&
                   object1->name() == object2->name();
        }

        std::hash<LocationSPtr> hashL;
        std::hash<std::string> hashString;
    };

private:
    Type mType;
    LocationSPtr mLocation;
    std::string mName;
};
}
