//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/fs/fs_file.h"
#include "doim/tag/tag_expression.h"
#include "err/err.h"
#include <str>

namespace parser
{
class DbsConfigParser
{
public:
    ECode parse(const doim::FsFileSPtr& dbsFile);

    ECode parse(const string& content);

    // Temporary
    doim::TagExpressionSPtr mTagExpression;
};
}
