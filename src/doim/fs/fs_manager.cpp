//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_manager.h"

#include "err/err_assert.h"

#include "const/constants.h"

#include <__hash_table>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>

namespace doim
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

    auto pos = begin;

    FsDirectory::Builder builder;

    auto parent = base;
    while (pos < end)
    {
        const auto& next = std::find(pos, end, slash());

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
                builder.ensure();
                builder.set_parent(parent);
                builder.set_name(name);
                const auto& emplace = mDirectories.emplace(builder.reference());
                parent = *emplace.first;
                if (emplace.second)
                    builder.reset();
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

    const auto& directory = obtainDirectory(base, begin, pos);
    if (directory == nullptr)
        return FsFileSPtr();

    auto working = std::make_shared<FsFile>(directory, std::string(pos, end));
    return *mFiles.emplace(working).first;
}
} // namespace doim
