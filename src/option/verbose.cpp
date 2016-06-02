//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "option/verbose.h"
#include <functional>

namespace opt
{
VerboseSPtr gVerbose = im::InitializationManager::subscribe(gVerbose);

Verbose::Verbose()
{
    auto task = doim::TagExpression::unique(doim::TagExpression::ETurn::kOn,
                                            doim::TagSet::unique(doim::TagSet::make(
                                                doim::TagSet{doim::gTaskTag})),
                                            nullptr);

    auto parse = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                             doim::TagSet::unique(doim::TagSet::make(
                                                 doim::TagSet{doim::gParseTag})),
                                             task);

    auto crc = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                           doim::TagSet::unique(doim::TagSet::make(
                                               doim::TagSet{doim::gCrcTag})),
                                           parse);

    auto db = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                          doim::TagSet::unique(doim::TagSet::make(
                                              doim::TagSet{doim::gDbTag})),
                                          crc);

    auto sys = doim::TagExpression::unique(doim::TagExpression::ETurn::kOff,
                                           doim::TagSet::unique(doim::TagSet::make(
                                               doim::TagSet{doim::gSysTag})),
                                           db);

    mTagExpression = sys;
}

bool Verbose::isVisible(const doim::TagSetSPtr& tags)
{
    return mTagExpression->match(tags);
}
} // namespace opt
