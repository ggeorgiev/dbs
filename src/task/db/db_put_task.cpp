//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err_assert.h"
#include <str>

namespace task
{
DbPutTask::DbPutTask(const doim::DbKeySPtr& key, const doim::DbValueSPtr& value)
    : Base(key)
    , mValue(value)
{
    ASSERT(key->isUnique());
}

ECode DbPutTask::operator()()
{
    EHTest(db::gDatabase->put(key()->toString(), value()->bytes()));
    EHEnd;
}

string DbPutTask::description() const
{
    return "Update key " + key()->toString();
}

} // namespace task
