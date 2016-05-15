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
        return doim::FsDirectory::find(base, directory);
    };

    return find(traceFile(traceDirectory, base, file));
}

FsFileSPtr Manager::obtainFile(const FsDirectorySPtr& base, const string_view& file)
{
    auto traceDirectory = [this](const FsDirectorySPtr& base,
                                 const string_view& directory) -> FsDirectorySPtr {
        return doim::FsDirectory::obtain(base, directory);
    };

    return unique(traceFile(traceDirectory, base, file));
}

} // namespace doim
