//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/protobuf/protobuf_file.h"
#include "err/err_assert.h"

namespace doim
{
ProtobufFile::ProtobufFile(const FsFileSPtr& file)
    : Element(file)
{
    ASSERT(file->isUnique());
}
}
