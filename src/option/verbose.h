//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_file.h"
#include "doim/tag/tag.h"
#include "doim/tag/tag_expression.h"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace opt
{
class Verbose
{
public:
    static constexpr int rank()
    {
        return doim::Tag::rank() + im::InitializationManager::step();
    }

    Verbose();

    ECode config(const doim::FsFileSPtr& file);

    bool isVisible(const doim::TagSetSPtr& tags);

private:
    doim::TagExpressionSPtr mTagExpression;
};

typedef shared_ptr<Verbose> VerboseSPtr;
extern VerboseSPtr gVerbose;
}
