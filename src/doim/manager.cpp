//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "err/err_assert.h"
#include "const/constants.h"
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

    FsDirectory::Builder builder;
    auto parent = directory.front() == slash() ? nullptr : base;

    ASSERT(parent != nullptr || directory.front() == slash());
    ASSERT(parent == nullptr || directory.front() != slash());

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

    const auto& parent =
        obtainCorrespondingDirectory(directory->parent(), fromDirectory, toDirectory);
    return obtainDirectory(parent, directory->name());
}

FsFileSPtr Manager::createFile(const FsDirectorySPtr& base,
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

    return std::make_shared<FsFile>(directory,
                                    std::string(file.begin() + pos, file.end()));
}

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory)
{
    out << "type:";
    switch (directory.type())
    {
        case CxxIncludeDirectory::Type::kUser:
            out << "user";
            break;
        case CxxIncludeDirectory::Type::kSystem:
            out << "system";
            break;
    }
    out << std::endl << "directory: " << directory.directory()->path() << std::endl;
    for (const auto& header : *directory.headerFiles())
        out << *header << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectorySet& directories)
{
    for (const auto& directory : directories)
        out << *directory << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const CxxHeader& header)
{
    out << "type:";
    switch (header.type())
    {
        case CxxHeader::Type::kUser:
            out << "user";
            break;
        case CxxHeader::Type::kSystem:
            out << "system";
            break;
    }
    out << std::endl << "file: " << header.file()->path() << std::endl;
    return out;
}

} // namespace doim
