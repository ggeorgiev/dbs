//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "db/database.h"
#include <sstream>

namespace task
{
ECode DbPutTask::operator()()
{
    EHTest(db::gDatabase->put(mKey->bytes(), mValue->bytes()));
    EHEnd;
}
} // namespace task
