//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag.h"
#include "doim/manager.h"
#include <functional>

namespace doim
{
TagSPtr Tag::global(const string& name, TagSPtr& tag)
{
    return Manager::global<Tag, string>(name, tag);
}

TagSPtr gBuildTag = Tag::global("build", gBuildTag);
TagSPtr gCoverageTag = Tag::global("coverage", gCoverageTag);
TagSPtr gIwyuTag = Tag::global("iwyu", gIwyuTag);

TagSPtr gTaskTag = Tag::global("task", gTaskTag);

TagSPtr gCrcTag = Tag::global("crc", gCrcTag);
TagSPtr gDbTag = Tag::global("db", gDbTag);
TagSPtr gSysTag = Tag::global("sys", gSysTag);

TagSPtr gRunTag = Tag::global("run", gRunTag);
TagSPtr gDoneTag = Tag::global("done", gDoneTag);
}
