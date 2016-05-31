//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/protobuf/protobuf_file.h"
#include "err/err_assert.h"

namespace doim
{
ProtobufFile::ProtobufFile(const FsDirectorySPtr& directory, const FsFileSPtr& file)
    : Element(directory, file)
{
    ASSERT(directory != nullptr && directory->isUnique());
    ASSERT(file != nullptr && file->isUnique());
}
}
