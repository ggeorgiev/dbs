//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "dom/protobuf/protobuf_plugin.h"
#include "doim/generic/attribute.h"
#include "doim/set.hpp"
#include "err/err.h"
#include <functional>
#include <shared_ptr>
#include <string>
#include <unordered>
#include <vector>

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
