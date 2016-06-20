//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_directory.h"
#include "doim/url/url.h"
#include "im/initialization_manager.hpp"
#include "log/log.h"
#include <shared_ptr>

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
