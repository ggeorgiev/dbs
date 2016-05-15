//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/db/db_key.h"
#include "doim/db/db_value.hpp"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class DbPutTask;

typedef shared_ptr<DbPutTask> DbPutTaskSPtr;

class DbPutTask : public Element<DbPutTask, doim::DbKeySPtr>
{
public:
    DbPutTask(const doim::DbKeySPtr& key, const doim::DbValueSPtr& value);

    doim::DbKeySPtr key() const
    {
        return std::get<0>(mArgs);
    }

    doim::DbValueSPtr value() const
    {
        return mValue;
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gDbTag};
    }

    ECode operator()() override;
    string description() const override;

private:
    doim::DbValueSPtr mValue;
};
}
