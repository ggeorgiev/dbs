//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include <iosfwd>
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class Tag;

typedef shared_ptr<Tag> TagSPtr;
typedef unordered_set<TagSPtr> TagSet;
typedef shared_ptr<TagSet> TagSetSPtr;

class Tag : public Base<Tag, string>
{
public:
    static TagSPtr global(const string& name, TagSPtr& tag);

    Tag(const string& name)
        : Base(name)
    {
    }

    const string& name() const
    {
        return std::get<0>(mArgs);
    }
};

extern TagSPtr gBuildTag;
extern TagSPtr gCoverageTag;
extern TagSPtr gIwyuTag;

extern TagSPtr gTaskTag;

extern TagSPtr gCrcTag;
extern TagSPtr gDbTag;
extern TagSPtr gSysTag;

extern TagSPtr gRunTag;
extern TagSPtr gDoneTag;
}
