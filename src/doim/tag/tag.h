//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"
#include "doim/set.hpp"
#include <boost/hana/for_each.hpp>
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

extern TagSPtr gCrcTag;
extern TagSPtr gDependTag;

extern TagSPtr gDbTag;
extern TagSPtr gParseTag;
extern TagSPtr gSysTag;

extern TagSPtr gTaskTag;
extern TagSPtr gRunTag;
extern TagSPtr gDoneTag;

extern TagSetSPtr gRunTagSet;
extern TagSetSPtr gDoneTagSet;

extern TagSPtr gLoadTag;
extern TagSPtr gSaveTag;

extern TagSPtr gCxxTag;
extern TagSPtr gObjTag;
extern TagSPtr gProgramTag;
extern TagSPtr gProtobufTag;
}
