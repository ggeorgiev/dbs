//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "const/constants.h"

#include <memory>
#include <string>

#include <iostream>

namespace dom
{
class FsDirectory;
typedef std::shared_ptr<FsDirectory> FsDirectorySPtr;

class FsDirectory
{
public:
    std::string path(const FsDirectorySPtr& directory)
    {
        size_t dlevel = (directory == nullptr) ? 0 : directory->level();
        size_t clevel = level();

        FsDirectorySPtr base = directory;

        std::string path;
        while (dlevel > clevel)
        {
            path += "../";
            base = base->parent();
            --dlevel;
        }

        if (this == base.get())
            return path;

        calculate(directory, path.size(), path);
        return path;
    }
    const std::string& name() const { return mName; }
    void set_name(const std::string& name) { mName = name; }
    const FsDirectorySPtr& parent() const { return mParent; }
    void set_parent(const FsDirectorySPtr& parent) { mParent = parent; }
    size_t level()
    {
        size_t level = 1;
        auto current = parent();
        while (current != nullptr)
        {
            ++level;
            current = current->parent();
        }
        return level;
    }

private:
    void calculate(const FsDirectorySPtr& directory, size_t length, std::string& path)
    {
        length += name().size() + 1;

        if (parent() == directory)
            path.reserve(length);
        else
            parent()->calculate(directory, length, path);

        path += name();
        path += slash();
    }

    std::string mName;
    FsDirectorySPtr mParent;
};
}
