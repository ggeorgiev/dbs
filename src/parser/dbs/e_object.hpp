//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_directory.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/object.h"
#include "err/err.h"

namespace parser
{
struct Object
{
    Object(const doim::FsDirectorySPtr& location)
        : mLocation(location)
    {
    }

    auto setType(const doim::Object::EType& type)
    {
        return e_ref([this, type](I& i1, I& i2) { mObjType = type; });
    }

    auto set()
    {
        return e_ref([this](I& i1, I& i2) {
            auto size = std::distance(i1, i2);
            auto path = string_view(&*i1, std::distance(i1, i2));
            mObject = doim::Object::obtain(mObjType, mLocation, path);
        });
    }

    template <typename WS>
    auto rule(const WS& r_ws, const doim::Object::EType& type)
    {
        return r_empty() >> setType(type) & r_ws & r_path >> set();
    }

    doim::FsDirectorySPtr mLocation;
    doim::ObjectSPtr mObject;
    doim::Object::EType mObjType;
};

template <typename Ref>
struct ObjectRef
{
    ObjectRef(const doim::FsDirectorySPtr& location)
        : mObject(location)
    {
    }

    auto set()
    {
        return e_ref([this](I& i1, I& i2) { mObjectRef = Ref::obtain(mObject.mObject); });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return mObject.rule(r_ws, Ref::objectType()) >> set();
    }

    Object mObject;
    shared_ptr<Ref> mObjectRef;
};
}
