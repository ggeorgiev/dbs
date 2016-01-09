//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <rocksdb/db.h>
#include <iosfwd>
#include <memory>
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <experimental/string_view>

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

    ECode get(const std::experimental::string_view& key, std::string& value);
    ECode put(const std::experimental::string_view& key,
              const std::experimental::string_view& value);
    ECode erase(const std::experimental::string_view& key);

private:
    std::unique_ptr<rocksdb::DB> mRocksDb;
};

typedef std::shared_ptr<Database> DatabaseSPtr;

extern DatabaseSPtr gDatabase;
}
