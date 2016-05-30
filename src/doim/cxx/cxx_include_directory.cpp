//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_include_directory.h"
#include "doim/cxx/cxx_header.h"
#include "doim/set.hpp"
#include "err/err_assert.h"
#include "log/log.h"
#include <ostream>
#include <unordered>
#include <utility>

namespace doim
{
ECode CxxIncludeDirectory::findHeader(
    const string_view& header,
    const CxxIncludeDirectorySPtr& currentIncludeDirectory,
    const CxxIncludeDirectorySetSPtr& includeDirectories,
    CxxHeaderInfo& headerInfo)
{
    CxxHeaderInfo result;

    if (currentIncludeDirectory != nullptr)
        result = {currentIncludeDirectory->findHeader(header), currentIncludeDirectory};

    for (const auto& directory : *includeDirectories)
    {
        const auto& cxxHeader = directory->findHeader(header);
        if (cxxHeader == nullptr)
            continue;

        if (result.mHeader != nullptr)
            EHBan(kTooMany, header.to_string());

        DLOG("Found header {} in directory {}",
             cxxHeader->file()->path(),
             directory->directory()->path());

        result = {cxxHeader, directory};
    }

    if (result.mHeader == nullptr)
        EHBan(kNotFound, header.to_string());

    headerInfo = result;
    EHEnd;
}

ECode CxxIncludeDirectory::findHeader(
    const FsFileSPtr& header,
    const CxxIncludeDirectorySPtr& currentIncludeDirectory,
    const CxxIncludeDirectorySetSPtr& includeDirectories,
    CxxHeaderInfo& headerInfo)
{
    CxxHeaderInfo result;

    if (currentIncludeDirectory != nullptr)
        result = {currentIncludeDirectory->findHeader(header), currentIncludeDirectory};

    for (const auto& directory : *includeDirectories)
    {
        const auto& cxxHeader = directory->findHeader(header);
        if (cxxHeader == nullptr)
            continue;

        if (result.mHeader != nullptr)
            EHBan(kTooMany);

        DLOG("Found header {} in directory {}",
             cxxHeader->file()->path(),
             directory->directory()->path());

        result = {cxxHeader, directory};
    }

    if (result.mHeader == nullptr)
        EHBan(kNotFound);

    headerInfo = result;
    EHEnd;
}

template <typename H>
ECode findHeaderDeepTemplate(const H& header,
                             const CxxIncludeDirectorySPtr& currentIncludeDirectory,
                             const CxxIncludeDirectorySetSPtr& includeDirectories,
                             CxxIncludeDirectory::CxxHeaderInfo& headerInfo)
{
    CxxIncludeDirectorySet checked;
    CxxIncludeDirectorySet todo;
    if (currentIncludeDirectory != nullptr)
        todo.insert(currentIncludeDirectory);
    todo.insert(includeDirectories->begin(), includeDirectories->end());

    CxxIncludeDirectory::CxxHeaderInfo result;
    while (!todo.empty())
    {
        auto current = *todo.begin();
        todo.erase(todo.begin());
        checked.insert(current);

        for (const auto cxxHeader : *current->headerFiles())
        {
            for (auto includeDir : *cxxHeader->cxxIncludeDirectories())
            {
                if (checked.count(includeDir) == 0)
                    todo.insert(includeDir);
            }
        }

        auto cxxHeader = current->findHeader(header);
        if (cxxHeader == nullptr)
            continue;

        if (result.mHeader != nullptr)
            EHBan(kTooMany);

        DLOG("Deep found header {} in directory {}",
             cxxHeader->file()->path(),
             current->directory()->path());

        result = {cxxHeader, current};
    }

    if (result.mHeader == nullptr)
        EHBan(kNotFound);

    headerInfo = result;
    EHEnd;
}

ECode CxxIncludeDirectory::findHeaderDeep(
    const string_view& header,
    const CxxIncludeDirectorySPtr& currentIncludeDirectory,
    const CxxIncludeDirectorySetSPtr& includeDirectories,
    CxxHeaderInfo& headerInfo)
{
    return findHeaderDeepTemplate(header,
                                  currentIncludeDirectory,
                                  includeDirectories,
                                  headerInfo);
}

ECode CxxIncludeDirectory::findHeaderDeep(
    const FsFileSPtr& header,
    const CxxIncludeDirectorySPtr& currentIncludeDirectory,
    const CxxIncludeDirectorySetSPtr& includeDirectories,
    CxxHeaderInfo& headerInfo)
{
    return findHeaderDeepTemplate(header,
                                  currentIncludeDirectory,
                                  includeDirectories,
                                  headerInfo);
}

CxxIncludeDirectory::CxxIncludeDirectory(const EType type,
                                         const FsDirectorySPtr& directory,
                                         const CxxHeaderSetSPtr& headerFiles)
    : Element(type, directory, headerFiles)
{
    ASSERT(directory != nullptr && directory->isUnique());
    ASSERT(headerFiles != nullptr && headerFiles->isUnique());
}

void CxxIncludeDirectory::finally()
{
    for (const auto& header : *headerFiles())
    {
        ASSERT(mFiles.count(header->file()) == 0);
        mFiles[header->file()] = header;
    }
}

CxxHeaderSPtr CxxIncludeDirectory::findHeader(const string_view& header) const
{
    const auto& file = FsFile::find(directory(), header);
    if (file == nullptr)
        return nullptr;

    return findHeader(file);
}

CxxHeaderSPtr CxxIncludeDirectory::findHeader(const FsFileSPtr& file) const
{
    const auto& it = mFiles.find(file);
    if (it != mFiles.end())
        return it->second;
    return nullptr;
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
}
