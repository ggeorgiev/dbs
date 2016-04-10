//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class Tag;
typedef std::shared_ptr<Tag> TagSPtr;
typedef std::unordered_set<TagSPtr> TagSet;
typedef std::shared_ptr<TagSet> TagSetSPtr;

class Tag : public Base<Tag, std::string>
{
public:
    static TagSPtr global(const std::string& name, TagSPtr& tag);

    Tag(const std::string& name)
        : Base(name)
    {
    }

    const std::string& name() const
    {
        return std::get<0>(mArgs);
    }
};

extern TagSPtr gCrcTag;
extern TagSPtr gTaskTag;
extern TagSPtr gRunTag;
extern TagSPtr gDoneTag;
}
