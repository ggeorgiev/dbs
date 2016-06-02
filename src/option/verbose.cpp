//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "option/verbose.h"
#include "parser/dbs/dbs_config_parser.h"
#include <functional>

namespace opt
{
VerboseSPtr gVerbose = im::InitializationManager::subscribe(gVerbose);

Verbose::Verbose()
{
    parser::DbsConfigParser parser;
    parser.parse("verbose: -sys -db -crc -parse +task;");

    mTagExpression = parser.mTagExpression;
}

bool Verbose::isVisible(const doim::TagSetSPtr& tags)
{
    return mTagExpression->match(tags);
}
} // namespace opt
