//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
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

class Object : public Base<Object, int, std::string, LocationSPtr>
{
public:
    enum class Type
    {
        kCxxLibrary,
        kCxxProgram,
    };

    Object(const Type type, const std::string& name, const LocationSPtr& location);

    Type type() const
    {
        return static_cast<Type>(std::get<0>(mArgs));
    }

    const std::string& name() const
    {
        return std::get<1>(mArgs);
    }

    const LocationSPtr& location() const
    {
        return std::get<2>(mArgs);
    }
};
}
