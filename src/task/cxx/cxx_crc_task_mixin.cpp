//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_crc_task_mixin.h"
#include "doim/fs/fs_directory.h"
#include "doim/manager.h"
#include "err/err.h"
#include "log/log.h"

namespace task
{
doim::CxxHeaderSPtr CxxCrcTaskMixin::findInclude(
    const string_view& include, const doim::CxxIncludeDirectorySPtr& includeDirectory)
{
    if (includeDirectory == nullptr)
        return nullptr;

    const auto& file = doim::gManager->findFile(includeDirectory->directory(), include);
    if (file == nullptr)
        return nullptr;

    return includeDirectory->header(file);
}

ECode CxxCrcTaskMixin::findInclude(
    const string_view& include,
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
            EHBan(kTooMany, include.to_string());

        DLOG("Found header {} in directory {}",
             header->file()->path(),
             directory->directory()->path());

        result = std::make_pair(header, directory);
    }

    if (result.first == nullptr)
    {
        EHBan(kNotFound, include.to_string() /*,
              EHPtr(currentIncludeDirectory),
              EHPtr(includeDirectories)*/);
    }

    headerDirectory = result;
    EHEnd;
}
}
