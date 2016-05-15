//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/fs/fs_file.h"

#include "err/err_assert.h"
#include "const/constants.h"

namespace doim
{
template <typename Trace, typename TraceDirectory>
FsFileSPtr traceFile(Trace&& trace,
                     TraceDirectory&& traceDirectory,
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

    return trace(FsFile::make(directory, string(file.begin() + pos, file.end())));
}

FsFileSPtr FsFile::find(const FsDirectorySPtr& base, const string_view& file)
{
    auto trace = [](const FsFileSPtr& file) -> FsFileSPtr { return file->find(); };

    auto traceDirectory = [](const FsDirectorySPtr& base,
                             const string_view& directory) -> FsDirectorySPtr {
        return doim::FsDirectory::find(base, directory);
    };

    return traceFile(trace, traceDirectory, base, file);
}

FsFileSPtr FsFile::obtain(const FsDirectorySPtr& base, const string_view& file)
{
    auto trace = [](const FsFileSPtr& file) -> FsFileSPtr { return unique(file); };

    auto traceDirectory = [](const FsDirectorySPtr& base,
                             const string_view& directory) -> FsDirectorySPtr {
        return doim::FsDirectory::obtain(base, directory);
    };

    return traceFile(trace, traceDirectory, base, file);
}

FsFile::FsFile(const FsDirectorySPtr& directory, const string& name)
    : Element(directory, name)
{
    ASSERT(directory->isUnique());
}
}
