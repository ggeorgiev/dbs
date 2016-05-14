//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/generic/location.hpp"
#include <iosfwd>
#include <memory>
#include <tuple>
#include <type_traits>

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

namespace o_details
{
enum class EType
{
    kCxxLibrary,
    kCxxProgram,
};
}

class Object;
typedef std::shared_ptr<Object> ObjectSPtr;
typedef unordered_set<ObjectSPtr> ObjectSet;
typedef std::shared_ptr<ObjectSet> ObjectSetSPtr;

class Object : public Base<Object,
                           std::underlying_type<o_details::EType>::type,
                           std::string,
                           LocationSPtr>
{
public:
    typedef o_details::EType EType;

    Object(const EType type, const std::string& name, const LocationSPtr& location);

    EType type() const
    {
        return static_cast<EType>(std::get<0>(mArgs));
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
