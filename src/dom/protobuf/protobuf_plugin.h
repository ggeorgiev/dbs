//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/element.hpp"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <shared_ptr>
#include <unordered>

namespace dom
{
class ProtobufPlugin;
typedef shared_ptr<ProtobufPlugin> ProtobufPluginSPtr;
typedef unordered_set<ProtobufPluginSPtr> ProtobufPluginSet;
typedef shared_ptr<ProtobufPluginSet> ProtobufPluginSetSPtr;

class ProtobufPlugin : public Element<ProtobufPlugin>
{
public:
    ProtobufPlugin();

    doim::FsFileSPtr binary()
    {
        return mBinary;
    }

    // Updates
    ECode updateBinary(const doim::FsFileSPtr& binary);

private:
    doim::FsFileSPtr mBinary;
};
}
