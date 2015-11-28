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
    while (pos < end)
    {
        auto next = std::find(pos, end, slash());

        if (next != pos || parent == nullptr)
        {
            std::string name(pos, next);
            if (name == kParentDirectoryString)
            {
                if (parent == nullptr)
                    return parent;
                parent = parent->parent();
            }
            else if (name != kCurrentDirectoryString)
            {
                if (working == nullptr)
                    working = std::make_shared<FsDirectory>();
                working->set_parent(parent);
                working->set_name(name);

                parent = *mDirectories.emplace(working).first;
                if (parent == working)
                    working.reset();
            }
            if (next == end)
                return parent;
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
