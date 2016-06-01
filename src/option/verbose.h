//  Copyright © 2015-2016 George Georgiev. All rights reserved.
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
    Verbose();

    static bool match(const doim::TagSetSPtr& pattern, const doim::TagSetSPtr& tags);

    static bool match(const unordered_set<doim::TagSetSPtr>& pattern,
                      const doim::TagSetSPtr& tags);

    static constexpr int rank()
    {
        return doim::Tag::rank() + im::InitializationManager::step();
    }

    bool isVisible(const doim::TagSetSPtr& tags);

private:
    unordered_set<doim::TagSetSPtr> mOrSets;
    unordered_set<doim::TagSetSPtr> mNotOrSets;
};

typedef shared_ptr<Verbose> VerboseSPtr;

extern VerboseSPtr gVerbose;
}
