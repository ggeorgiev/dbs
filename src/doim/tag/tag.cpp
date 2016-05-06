//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag.h"
#include "doim/manager.h"
#include "im/initialization_manager.hpp"
#include <functional>

namespace doim
{
TagSPtr Tag::global(const std::string& name, TagSPtr& tag)
{
    im::InitializationManager::subscribe<
        TagSPtr>(Manager::initialization_rank() + im::InitializationManager::rank_step(),
                 [&tag]() -> bool {
                     tag = gManager->unique(tag);
                     return true;
                 },
                 nullptr);
    return std::make_shared<Tag>(name);
}

TagSPtr gBuildTag = Tag::global("build", gBuildTag);
TagSPtr gIwyuTag = Tag::global("iwyu", gIwyuTag);

TagSPtr gTaskTag = Tag::global("task", gTaskTag);

TagSPtr gCrcTag = Tag::global("crc", gCrcTag);
TagSPtr gDbTag = Tag::global("db", gDbTag);
TagSPtr gSysTag = Tag::global("sys", gSysTag);
;

TagSPtr gRunTag = Tag::global("run", gRunTag);
TagSPtr gDoneTag = Tag::global("done", gDoneTag);
}
