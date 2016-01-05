//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <rocksdb/db.h>
#include <iosfwd>
#include <memory>

#include "err/err.h"
#include "im/initialization_manager.hpp"

namespace db
{
class Database
{
public:
    static inline int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    ECode open(const std::string& file);
    void close();

private:
    std::unique_ptr<rocksdb::DB> mRocksDb;
};

typedef std::shared_ptr<Database> DatabaseSPtr;

extern DatabaseSPtr gDatabase;
}
