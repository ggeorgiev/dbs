//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "const/constants.h"
#include "err/err_assert.h"
#include <algorithm>
#include <functional>
#include <iosfwd>
#include <string>
#include <utility>

namespace doim
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);

ObjectSPtr Manager::obtainObject(const LocationSPtr& base,
                                 const Object::Type type,
                                 const std::experimental::string_view& object)
{
    auto pos = object.size();
    while (pos-- > 0)
    {
        if (object[pos] == slash())
            break;
    }
    ++pos;

    if (pos == object.size())
        return ObjectSPtr();

    auto location =
        obtainLocation(base, std::experimental::string_view(object.begin(), pos));
    if (location == nullptr)
        return ObjectSPtr();

    auto working =
        std::make_shared<Object>(type,
                                 std::string(object.begin() + pos, object.end()),
                                 location);
    return unique(working);
}

FsDirectorySPtr Manager::obtainDirectory(const FsDirectorySPtr& base,
                                         const std::experimental::string_view& directory)
{
    if (directory.empty())
        return base;

    ASSERT(base != nullptr || directory.front() == slash());
    ASSERT(base == nullptr || directory.front() != slash());

    FsDirectory::Builder builder;
    auto parent = base;

    auto pos = directory.begin();
    while (pos < directory.end())
    {
        const auto& next = std::find(pos, directory.end(), slash());

        if (next != pos || parent == nullptr)
        {
            std::string name(pos, next);
            if (name == kParentDirectoryString)
            {
                if (parent == nullptr)
                    return nullptr;
                parent = parent->parent();
            }
            else if (name != kCurrentDirectoryString)
            {
                builder.ensure();
                builder.set_parent(parent);
                builder.set_name(name);
                const auto& insert =
                    ManagerMixin<FsDirectory>::mMixinObjects.insert(builder.reference());
                parent = *insert.first;
                if (insert.second)
                    builder.reset();
            }
            if (next == directory.end())
                return parent;
        }

        pos = next + 1;
    }

    if (parent == nullptr)
        parent = std::make_shared<FsDirectory>();

    return unique(parent);
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
                               const std::experimental::string_view& file)
{
    auto pos = file.size();
    while (pos-- > 0)
    {
        if (file[pos] == slash())
            break;
    }
    ++pos;

    if (pos == file.size())
        return FsFileSPtr();

    const auto& directory =
        obtainDirectory(base, std::experimental::string_view(file.begin(), pos));
    if (directory == nullptr)
        return FsFileSPtr();

    auto working =
        std::make_shared<FsFile>(directory, std::string(file.begin() + pos, file.end()));
    return unique(working);
}

} // namespace doim
