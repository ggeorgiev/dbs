//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "option/verbose.h"

namespace opt
{
VerboseSPtr gVerbose = im::InitializationManager::subscribe(gVerbose);

bool Verbose::isVisible(const doim::TagSet& tags)
{
    if (tags.count(doim::gCrcTag) > 0)
        return false;
    return true;
}

} // namespace db
