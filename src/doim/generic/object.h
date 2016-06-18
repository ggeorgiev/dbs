//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/generic/location.hpp"
#include "doim/element.hpp"
#include <boost/hana/for_each.hpp>
#include <memory>
#include <str>
#include <string_view>
#include <tuple>
#include <unordered>

namespace doim
{
/*
 * Generic Object
 *
 * Generic objects are identified by name and location. For sake of simplicity and code
 * efficiency the location of the object is the shared with the fs directory
 * infrastructure.
 */

struct ObjectEnums
{
    enum class EType
    {
        kCxxLibrary,
        kCxxProgram,
        kProtobufPlugin,
    };
};

class Object;
typedef shared_ptr<Object> ObjectSPtr;
typedef unordered_set<ObjectSPtr> ObjectSet;
typedef shared_ptr<ObjectSet> ObjectSetSPtr;

class Object : public ObjectEnums,
               public Element<Object, ObjectEnums::EType, LocationSPtr, string>
{
public:
    static ObjectSPtr obtain(const Object::EType type,
                             const LocationSPtr& base,
                             const string_view& object);

    Object(const EType type, const LocationSPtr& location, const string& name);

    EType type() const
    {
        return std::get<0>(mArgs);
    }

    const LocationSPtr& location() const
    {
        return std::get<1>(mArgs);
    }

    const string& name() const
    {
        return std::get<2>(mArgs);
    }
};
}
