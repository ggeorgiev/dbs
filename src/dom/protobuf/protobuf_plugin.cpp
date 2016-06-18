//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/protobuf/protobuf_plugin.h"
#include "err/err.h"

namespace dom
{
ProtobufPlugin::ProtobufPlugin()
{
}

ECode ProtobufPlugin::updateBinary(const doim::FsFileSPtr& binary)
{
    mBinary = binary;
    EHEnd;
}
}
