//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "option/verbose.h"
#include <functional>

namespace opt
{
VerboseSPtr gVerbose = im::InitializationManager::subscribe(gVerbose);

Verbose::Verbose()
{
    mNotOrSets.insert(
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gParseTag})));
    mNotOrSets.insert(
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gCrcTag})));
    mNotOrSets.insert(
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gDbTag})));
    mNotOrSets.insert(
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gSysTag})));

    mOrSets.insert(
        doim::TagSet::unique(doim::TagSet::make(doim::TagSet{doim::gTaskTag})));
}

bool Verbose::match(const doim::TagSetSPtr& pattern, const doim::TagSetSPtr& tags)
{
    for (const auto& tag : *pattern)
    {
        if (tags->count(tag) == 0)
            return false;
    }
    return true;
}

bool Verbose::match(const unordered_set<doim::TagSetSPtr>& sets,
                    const doim::TagSetSPtr& tags)
{
    for (const auto& set : sets)
    {
        if (match(set, tags))
            return true;
    }
    return false;
}

bool Verbose::isVisible(const doim::TagSetSPtr& tags)
{
    if (match(mNotOrSets, tags))
        return false;
    if (match(mOrSets, tags))
        return true;
    return true;
}
} // namespace opt
