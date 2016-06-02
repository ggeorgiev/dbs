//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tag/tag.h"
#include "doim/tag/tag_expression.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace opt
{
class Verbose
{
public:
    Verbose();

    static constexpr int rank()
    {
        return doim::Tag::rank() + im::InitializationManager::step();
    }

    bool isVisible(const doim::TagSetSPtr& tags);

private:
    doim::TagExpressionSPtr mTagExpression;
};

typedef shared_ptr<Verbose> VerboseSPtr;
extern VerboseSPtr gVerbose;
}
