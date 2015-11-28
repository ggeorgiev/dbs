//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "dom/fs/fs_manager.h"

#include "err/err_assert.h"

#include "const/constants.h"

#include <__hash_table>
#include <algorithm>
#include <functional>
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

                auto emplace = mDirectories.emplace(working);
                parent = *emplace.first;
                if (emplace.second)
                    working.reset();
            }
            if (next == end)
                return parent;
        }

        pos = next + 1;
    }

    if (parent == nullptr)
        parent = std::make_shared<FsDirectory>();

    return *mDirectories.emplace(parent).first;
}

FsFileSPtr FsManager::obtainFile(const FsDirectorySPtr& base,
                                 const std::string::const_iterator& begin,
                                 const std::string::const_iterator& end)
{
    auto pos = end;
    while (--pos >= begin)
    {
        if (*pos == slash())
            break;
    }
    ++pos;

    if (pos == end)
        return FsFileSPtr();

    auto directory = obtainDirectory(base, begin, pos);
    if (directory == nullptr)
        return FsFileSPtr();

    auto working = std::make_shared<FsFile>();
    working->set_directory(directory);
    working->set_name(std::string(pos, end));

    return *mFiles.emplace(working).first;
}
}
