//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tag/tag.h"
#include "im/initialization_manager.hpp"
#include <memory>

namespace opt
{
class Verbose
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    bool isVisible(const doim::TagSet& tags);
};

typedef std::shared_ptr<Verbose> VerboseSPtr;

extern VerboseSPtr gVerbose;
}
