//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/tag/tag.h"
#include "doim/url/url.h"
#include "err/err.h"
#include <fmt/format.h>
#include <boost/hana/for_each.hpp>
#include <shared_ptr>
#include <str>
#include <tuple>

namespace task
{
class GitFetchTask;
typedef shared_ptr<GitFetchTask> GitFetchTaskSPtr;

class GitFetchTask
    : public Element<GitFetchTask, doim::FsDirectorySPtr, doim::UrlSPtr, string>
{
public:
    GitFetchTask(const doim::FsDirectorySPtr& directory,
                 const doim::UrlSPtr& url,
                 const string checkout);

    bool check() const;

    doim::FsDirectorySPtr directory() const
    {
        return std::get<0>(mArgs);
    }

    doim::UrlSPtr url() const
    {
        return std::get<1>(mArgs);
    }

    string checkout() const
    {
        return std::get<2>(mArgs);
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;
};
}
