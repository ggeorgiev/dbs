//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "doim/db/db_key.h"
#include "doim/db/db_value.hpp"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <boost/hana/for_each.hpp>
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

    bool check() const;

    doim::DbKeySPtr key() const
    {
        return std::get<0>(mArgs);
    }

    doim::DbValueSPtr value() const
    {
        return mValue;
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;

private:
    doim::DbValueSPtr mValue;
};
}
