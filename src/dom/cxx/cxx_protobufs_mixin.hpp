//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include "log/log.h"
#include "log/log_function.hpp"
#include <memory>
#include <set>
#include <unordered_set>

namespace dom
{
template <typename T>
class ProtobufsMixin
{
public:
    typedef T Subject;

    ECode updateProtobufsList(doim::FsFileSet& files)
    {
        mProtobufsList.swap(files);
        EHEnd;
    }

    const doim::FsFileSet& protobufsList() const
    {
        return mProtobufsList;
    }

private:
    doim::FsFileSet mProtobufsList;
};
}
