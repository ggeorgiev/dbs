//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "db/database.h"
#include <sstream>
#include <string>

namespace task
{
DbPutTask::DbPutTask(const doim::DbKeySPtr& key, const doim::DbValueSPtr& value)
    : tpool::Task(0)
    , mKey(key)
    , mValue(value)
{
}

ECode DbPutTask::operator()()
{
    EHTest(db::gDatabase->put(mKey->bytes(), mValue->bytes()));
    EHEnd;
}

std::string DbPutTask::description() const
{
    return "Update key " + mKey->bytes();
}

} // namespace task
