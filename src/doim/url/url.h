//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/set.hpp"
#include <boost/hana/for_each.hpp>
#include <functional>
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class Url;
typedef shared_ptr<Url> UrlSPtr;
typedef Set<Url> UrlSet;
typedef shared_ptr<UrlSet> UrlSetSPtr;

class Url : public Element<Url, string>
{
public:
    Url(const string& path)
        : Element(path)
    {
    }

    const string& path() const
    {
        return std::get<0>(mArgs);
    }
};
}
