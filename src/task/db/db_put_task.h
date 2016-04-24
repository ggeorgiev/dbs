//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/base.hpp"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "err/err.h"
#include <cppformat/format.h>
#include <spdlog/details/line_logger_impl.h>
#include <iosfwd>
#include <memory>
#include <tuple>

namespace task
{
class DbPutTask;

typedef std::shared_ptr<DbPutTask> DbPutTaskSPtr;

class DbPutTask : public Base<DbPutTask, doim::DbKeySPtr>
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
        return doim::TagSet{doim::gDbTag};
    }

    ECode operator()() override;
    std::string description() const override;

private:
    doim::DbValueSPtr mValue;
};
}
