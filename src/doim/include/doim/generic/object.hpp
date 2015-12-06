//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/generic/location.hpp"

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

private:
    Type mType;
    LocationSPtr mLocation;
    std::string mName;
};

typedef std::shared_ptr<Object> ObjectSPtr;
}
