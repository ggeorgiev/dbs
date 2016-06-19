//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_directory.hpp"
#include "doim/fs/fs_file.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
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

    auto cxxLibrary()
    {
        return e_ref(
            [this](I& i1, I& i2) { mObjType = doim::Object::EType::kCxxLibrary; });
    }

    auto cxxProgram()
    {
        return e_ref(
            [this](I& i1, I& i2) { mObjType = doim::Object::EType::kCxxProgram; });
    }

    auto depository()
    {
        return e_ref(
            [this](I& i1, I& i2) { mObjType = doim::Object::EType::kDepository; });
    }

    void operator()(I& i1, I& i2)
    {
        auto size = std::distance(i1, i2);
        auto path = string_view(&*i1, std::distance(i1, i2));
        mObject = doim::Object::obtain(mObjType, mLocation, path);
    }

    doim::FsDirectorySPtr mLocation;
    doim::ObjectSPtr mObject;
    doim::Object::EType mObjType;
};
}
