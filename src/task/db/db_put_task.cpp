//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "doim/manager.h"
#include "db/database.h"
#include "err/err_assert.h"
#include <string>

namespace task
{
DbPutTask::DbPutTask(const doim::DbKeySPtr& key, const doim::DbValueSPtr& value)
    : Base(key)
    , mValue(value)
{
    ASSERT(doim::gManager->isUnique(key));
}

ECode DbPutTask::operator()()
{
    EHTest(db::gDatabase->put(key()->bytes(), value()->bytes()));
    EHEnd;
}

std::string DbPutTask::description() const
{
    return "Update key " + key()->bytes();
}

} // namespace task
