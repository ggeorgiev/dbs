//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_crc_task_mixin.h"
#include "doim/fs/fs_directory.h"
#include "doim/manager.h"
#include "log/log.h"
#include <spdlog/details/line_logger_impl.h>
#include <sstream>

namespace task
{
doim::CxxHeaderSPtr CxxCrcTaskMixin::findInclude(
    const std::experimental::string_view& include,
    const doim::CxxIncludeDirectorySPtr& includeDirectory)
{
    if (includeDirectory == nullptr)
        return nullptr;

    const auto& file = doim::gManager->findFile(includeDirectory->directory(), include);
    if (file == nullptr)
        return nullptr;

    return includeDirectory->header(file);
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

        DLOG("Found header {} in directory {}",
             header->file()->path(),
             directory->directory()->path());

        result = std::make_pair(header, directory);
    }

    if (result.first == nullptr)
    {
        EHBan(kNotFound,
              include,
              EHPtr(currentIncludeDirectory),
              EHPtr(includeDirectories));
    }

    headerDirectory = result;
    EHEnd;
}
}
