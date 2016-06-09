//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <str>
#include <tuple>

namespace doim
{
class Tag;
typedef shared_ptr<Tag> TagSPtr;
typedef Set<Tag> TagSet;
typedef shared_ptr<TagSet> TagSetSPtr;

class Tag : public Element<Tag, string>
{
public:
    Tag(const string& name)
        : Element(name)
    {
    }

    const string& name() const
    {
        return std::get<0>(mArgs);
    }
};

extern TagSPtr gTaskTag;

extern TagSPtr gCrcTag;
extern TagSPtr gDbTag;
extern TagSPtr gParseTag;
extern TagSPtr gSysTag;

extern TagSPtr gRunTag;
extern TagSPtr gDoneTag;

extern TagSPtr gLoadTag;
extern TagSPtr gSaveTag;
}
