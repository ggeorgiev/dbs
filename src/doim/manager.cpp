//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/manager.h"

#include <__hash_table>
#include <algorithm>
#include <functional>
#include <string>
#include <utility>

#include "const/constants.h"
#include "err/err_assert.h"

namespace doim
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);

ObjectSPtr Manager::obtainObject(const LocationSPtr& base,
                                 const Object::Type type,
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
        return ObjectSPtr();

    auto location = obtainLocation(base, begin, pos);
    if (location == nullptr)
        return ObjectSPtr();

    auto working = std::make_shared<Object>(type, std::string(pos, end), location);
    return *mObjects.insert(working).first;
}

FsDirectorySPtr Manager::obtainDirectory(const FsDirectorySPtr& base,
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
                const auto& insert = mDirectories.insert(builder.reference());
                parent = *insert.first;
                if (insert.second)
                    builder.reset();
            }
            if (next == end)
                return parent;
        }

        pos = next + 1;
    }

    if (parent == nullptr)
        parent = std::make_shared<FsDirectory>();

    return *mDirectories.insert(parent).first;
}

FsDirectorySPtr Manager::obtainCorrespondingDirectory(
    const FsDirectorySPtr& directory,
    const FsDirectorySPtr& fromDirectory,
    const FsDirectorySPtr& toDirectory)
{
    ASSERT(directory != nullptr);

    if (directory == fromDirectory)
        return toDirectory;

    auto parent =
        obtainCorrespondingDirectory(directory->parent(), fromDirectory, toDirectory);
    return obtainDirectory(parent, directory->name());
}

FsFileSPtr Manager::obtainFile(const FsDirectorySPtr& base,
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
    return *mFiles.insert(working).first;
}

} // namespace doim
