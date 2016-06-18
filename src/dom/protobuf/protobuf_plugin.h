//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/cxx/cxx_files_mixin.hpp"
#include "dom/cxx/cxx_headers_mixin.hpp"
#include "dom/cxx/protobufs_mixin.hpp"
#include "dom/element.hpp"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "dp/handle.hpp"
#include "dp/map_container.hpp"
#include "dp/memoization.hpp"
#include "dp/solitary_container.hpp"
#include "err/err.h"
#include <boost/hana/for_each.hpp>
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
