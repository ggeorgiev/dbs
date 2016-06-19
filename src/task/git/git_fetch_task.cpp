//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/git/git_fetch_task.h"
#include "task/git/git2.h"
#include "task/tpool.h"
#include "log/log.h"
#include <git2.h>

namespace task
{
GitFetchTask::GitFetchTask(const doim::FsDirectorySPtr& directory,
                           const doim::UrlSPtr& url,
                           const string checkout)
    : Element(directory, url, checkout)
{
}

bool GitFetchTask::check() const
{
    return directory() != nullptr && directory()->isUnique() && url() != nullptr &&
           url()->isUnique();
}

static doim::TagSetSPtr gTags = doim::TagSet::global({}, gTags);

doim::TagSetSPtr GitFetchTask::tags() const
{
    return gTags;
}

ECode GitFetchTask::operator()()
{
    EHEnd;
}

string GitFetchTask::description() const
{
    return "Fetch git repo";
}

} // namespace task
