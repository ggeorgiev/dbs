//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/base.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <tuple>
#include <unordered>

namespace doim
{
class Tag;

typedef shared_ptr<Tag> TagSPtr;
typedef NewObjectSet<Tag> TagSet;
typedef shared_ptr<TagSet> TagSetSPtr;

class Tag : public Base<Tag, string>
{
public:
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
