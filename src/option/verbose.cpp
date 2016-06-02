//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "option/verbose.h"
#include "parser/dbs/dbs_config_parser.h"
#include <boost/filesystem/operations.hpp>
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

ECode Verbose::config(const doim::FsFileSPtr& file)
{
    const auto& path = file->path();
    if (!boost::filesystem::exists(path))
        EHEnd;

    parser::DbsConfigParser parser;
    parser.parse(file);

    mTagExpression = parser.mTagExpression;
    EHEnd;
}

bool Verbose::isVisible(const doim::TagSetSPtr& tags)
{
    return mTagExpression->match(tags);
}
} // namespace opt
