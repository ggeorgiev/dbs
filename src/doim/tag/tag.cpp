//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag.h"

namespace doim
{
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
