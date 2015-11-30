//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/generic/location.hpp"

#include <memory>
#include <string>

namespace dom
{
/*
 * Generic Object
 *
 * Generic objects are identified by name and location. For sake of simplicity and code efficiency
 * the location of the object is the shared with the fs directory infrastructure.
 *
 */

class Object
{
public:
    enum class Type
    {
        kInvalid = 0,
        kCppLibrary,
        kCppProgram,
    };

    Object() : mType(Type::kInvalid) {}
    Type type() { return mType; }
    void set_type(const Type type) { mType = type; }
    const std::string& name() const { return mName; }
    void set_name(const std::string& name) { mName = name; }
    const LocationSPtr& location() const { return mLocation; }
    void set_location(const LocationSPtr& location) { mLocation = location; }
private:
    Type mType;
    LocationSPtr mLocation;
    std::string mName;
};

typedef std::shared_ptr<Object> ObjectSPtr;
}
