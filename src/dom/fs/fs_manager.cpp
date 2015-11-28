//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/fs/fs_manager.h"

#include "err/err_assert.h"

#include "const/constants.h"

#include <__hash_table>
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

namespace dom
{
FsManagerSPtr gFsManager = im::InitializationManager::subscribe(gFsManager);

FsDirectorySPtr FsManager::obtainDirectory(const FsDirectorySPtr& base,
                                           const std::string::const_iterator& begin,
                                           const std::string::const_iterator& end)
{
    if (begin == end)
        return base;

    ASSERT(base != nullptr || *begin == slash());
    ASSERT(base == nullptr || *begin != slash());

    FsDirectorySPtr working;

    auto pos = begin;

    auto parent = base;
    while (pos != end)
    {
        auto next = std::find(pos, end, slash());

        if (next != pos || parent == nullptr)
        {
            if (working == nullptr)
                working = std::make_shared<FsDirectory>();
            working->set_parent(parent);
            working->set_name(std::string(pos, next));

            parent = *mDirectories.emplace(working).first;

            if (next == end)
                return parent;

            if (parent == working)
                working.reset();
        }

        pos = next + 1;
    }

    if (parent == nullptr)
    {
        parent = std::make_shared<FsDirectory>();
    }

    return *mDirectories.emplace(parent).first;
}
}
