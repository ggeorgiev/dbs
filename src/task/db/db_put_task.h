//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.hpp"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "err/err.h"
#include <boost/functional/hash/hash.hpp>
#include <iosfwd>
#include <memory>
#include <stddef.h>

namespace task
{
class DbPutTask;

typedef std::shared_ptr<DbPutTask> DbPutTaskSPtr;

class DbPutTask : public tpool::Task
{
public:
    DbPutTask(const doim::DbKeySPtr& key, const doim::DbValueSPtr& value);

    ECode operator()() override;
    std::string description() const override;

    struct Hasher
    {
        std::size_t operator()(const DbPutTaskSPtr& task) const;
        bool operator()(const DbPutTaskSPtr& task1, const DbPutTaskSPtr& task2) const;

        std::hash<doim::DbKeySPtr> hashDK;
        std::hash<doim::DbValueSPtr> hashDV;
    };

private:
    doim::DbKeySPtr mKey;
    doim::DbValueSPtr mValue;
};

inline std::size_t DbPutTask::Hasher::operator()(const DbPutTaskSPtr& task) const
{
    auto seed = hashDK(task->mKey);
    boost::hash_combine(seed, hashDV(task->mValue));
    return seed;
}

inline bool DbPutTask::Hasher::operator()(const DbPutTaskSPtr& task1,
                                          const DbPutTaskSPtr& task2) const
{
    return task1->mKey == task2->mKey && task1->mValue == task2->mValue;
}
}
