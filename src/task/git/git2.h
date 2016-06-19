//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "task/element_manager.hpp"
#include "doim/fs/fs_directory.h"
#include "doim/sys/sys_command.h"
#include "doim/tag/tag.h"
#include "doim/url/url.h"
#include "err/err.h"
#include "rtti/class_rtti.hpp"
#include <fmt/format.h>
#include <boost/hana/for_each.hpp>
#include <functional>
#include <shared_ptr>
#include <str>
#include <tuple>

namespace task
{
class Git2;
typedef shared_ptr<Git2> Git2SPtr;

class Git2
{
public:
    static constexpr int rank()
    {
        return dbslog::rank() + im::InitializationManager::step();
    }
    Git2();
    ~Git2();

    doim::UrlSPtr url(const doim::FsDirectorySPtr& repoDir);
};

extern Git2SPtr gGit;
}
