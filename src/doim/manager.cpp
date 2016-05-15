//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "err/err_assert.h"
#include "const/constants.h"
#include <algorithm>
#include <functional>
#include <shared_ptr>
#include <str>

namespace doim
{
ManagerSPtr gManager = im::InitializationManager::subscribe(gManager);

ObjectSPtr Manager::obtainObject(const LocationSPtr& base,
                                 const Object::EType type,
                                 const string_view& object)
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

    auto location = obtainLocation(base, string_view(object.begin(), pos));
    if (location == nullptr)
        return ObjectSPtr();

    auto working =
        Object::make(type, string(object.begin() + pos, object.end()), location);
    return unique(working);
}

template <typename Trace>
FsDirectorySPtr traceDirectory(Trace&& trace,
                               const FsDirectorySPtr& base,
                               const string_view& directory)
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
            string name(pos, next);
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

                const auto& current = builder.reference();
                parent = trace(current);

                if (parent == nullptr)
                    return parent;

                if (parent == current)
                    builder.reset();
            }
            if (next == directory.end())
                return parent;
        }

        pos = next + 1;
    }

    if (parent == nullptr)
        parent = FsDirectory::make();

    return trace(parent);
}

FsDirectorySPtr Manager::findDirectory(const FsDirectorySPtr& base,
                                       const string_view& directory)
{
    auto trace = [this](const FsDirectorySPtr& directory) -> FsDirectorySPtr {
        return find(directory);
    };

    return traceDirectory(trace, base, directory);
}

FsDirectorySPtr Manager::obtainDirectory(const FsDirectorySPtr& base,
                                         const string_view& directory)
{
    auto trace = [this](const FsDirectorySPtr& directory) -> FsDirectorySPtr {
        return unique(directory);
    };

    return traceDirectory(trace, base, directory);
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

template <typename TraceDirectory>
FsFileSPtr traceFile(TraceDirectory&& traceDirectory,
                     const FsDirectorySPtr& base,
                     const string_view& file)
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

    const auto& directory = traceDirectory(base, string_view(file.begin(), pos));
    if (directory == nullptr)
        return FsFileSPtr();

    return FsFile::make(directory, string(file.begin() + pos, file.end()));
}

FsFileSPtr Manager::findFile(const FsDirectorySPtr& base, const string_view& file)
{
    auto traceDirectory = [this](const FsDirectorySPtr& base,
                                 const string_view& directory) -> FsDirectorySPtr {
        return findDirectory(base, directory);
    };

    return find(traceFile(traceDirectory, base, file));
}

FsFileSPtr Manager::obtainFile(const FsDirectorySPtr& base, const string_view& file)
{
    auto traceDirectory = [this](const FsDirectorySPtr& base,
                                 const string_view& directory) -> FsDirectorySPtr {
        return obtainDirectory(base, directory);
    };

    return unique(traceFile(traceDirectory, base, file));
}

std::ostream& operator<<(std::ostream& out, const CxxIncludeDirectory& directory)
{
    out << "type:";
    switch (directory.type())
    {
        case CxxIncludeDirectory::EType::kUser:
            out << "user";
            break;
        case CxxIncludeDirectory::EType::kSystem:
            out << "system";
            break;
    }
    out << ", directory: " << directory.directory()->path() << std::endl;
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
    out << "header { type: ";
    switch (header.type())
    {
        case CxxHeader::EType::kUser:
            out << "user";
            break;
        case CxxHeader::EType::kSystem:
            out << "system";
            break;
    }
    out << ", file: " << header.file()->path() << "}" << std::endl;
    return out;
}

} // namespace doim
