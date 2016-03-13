//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_crc_task_mixin.h"
#include "doim/manager.h"
#include <unordered_set>

namespace task
{
doim::CxxHeaderSPtr CxxCrcTaskMixin::findInclude(
    const std::experimental::string_view& include,
    const doim::CxxIncludeDirectorySPtr& includeDirectory)
{
    if (includeDirectory == nullptr)
        return nullptr;

    const auto& file = doim::gManager->find(
        doim::gManager->createFile(includeDirectory->directory(), include));
    if (file == nullptr)
        return nullptr;

    const auto& header = doim::gManager->findCxxHeader(file);
    if (header == nullptr)
        return nullptr;

    if (includeDirectory->headerFiles()->count(header) == 0)
        return nullptr;

    return header;
}

ECode CxxCrcTaskMixin::findInclude(
    const std::experimental::string_view& include,
    const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory,
    const doim::CxxIncludeDirectorySetSPtr& includeDirectories,
    HeaderDirectory& headerDirectory)
{
    auto result = std::make_pair(findInclude(include, currentIncludeDirectory),
                                 currentIncludeDirectory);

    for (const auto& directory : *includeDirectories)
    {
        const auto& header = findInclude(include, directory);
        if (header == nullptr)
            continue;

        if (result.first != nullptr)
            EHBan(kTooMany, include);

        result = std::make_pair(header, directory);
    }

    if (result.first == nullptr)
        EHBan(kNotFound,
              include,
              EHPtr(currentIncludeDirectory),
              EHPtr(includeDirectories));

    headerDirectory = result;
    EHEnd;
}
}
