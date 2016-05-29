//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_file.h"
#include "doim/element.hpp"
#include "doim/set.hpp"
#include <memory>
#include <tuple>

namespace doim
{
class ProtobufFile;
typedef shared_ptr<ProtobufFile> ProtobufFileSPtr;
typedef Set<ProtobufFile> ProtobufFileSet;
typedef shared_ptr<ProtobufFileSet> ProtobufFileSetSPtr;

class ProtobufFile : public Element<ProtobufFile, FsFileSPtr>
{
public:
    ProtobufFile(const FsFileSPtr& file);

    const FsFileSPtr& file() const
    {
        return std::get<0>(mArgs);
    }
};
}
