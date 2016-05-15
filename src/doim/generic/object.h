//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/generic/location.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <type_traits>
#include <unordered>

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

struct ObjectEnums
{
    enum class EType
    {
        kCxxLibrary,
        kCxxProgram,
    };
};

class Object;
typedef shared_ptr<Object> ObjectSPtr;
typedef unordered_set<ObjectSPtr> ObjectSet;
typedef shared_ptr<ObjectSet> ObjectSetSPtr;

class Object : public ObjectEnums,
               public Base<Object, ObjectEnums::EType, string, LocationSPtr>
{
public:
    static ObjectSPtr obtain(const LocationSPtr& base,
                             const Object::EType type,
                             const string_view& object);

    Object(const EType type, const string& name, const LocationSPtr& location);

    EType type() const
    {
        return std::get<0>(mArgs);
    }

    const string& name() const
    {
        return std::get<1>(mArgs);
    }

    const LocationSPtr& location() const
    {
        return std::get<2>(mArgs);
    }
};
}
