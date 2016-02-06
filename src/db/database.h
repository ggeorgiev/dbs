//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <rocksdb/db.h>
#include <experimental/string_view>
#include <iosfwd>
#include <memory>

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

    template <typename V>
    ECode get(const std::experimental::string_view& key, V& value)
    {
        V dflt = V();

        std::string buffer;
        EHTest(get(key,
                   std::experimental::string_view(reinterpret_cast<const char*>(&dflt),
                                                  sizeof(dflt)),
                   buffer));
        value = *reinterpret_cast<const V*>(buffer.data());
        EHEnd;
    }

    template <typename V>
    ECode put(const std::experimental::string_view& key, const V& value)
    {
        EHTest(put(key,
                   std::experimental::string_view(reinterpret_cast<const char*>(&value),
                                                  sizeof(value))));
        EHEnd;
    }

    ECode get(const std::experimental::string_view& key,
              const std::experimental::string_view& dflt,
              std::string& value);
    ECode get(const std::experimental::string_view& key, std::string& value);
    ECode put(const std::experimental::string_view& key,
              const std::experimental::string_view& value);
    ECode erase(const std::experimental::string_view& key);

private:
    std::unique_ptr<rocksdb::DB> mRocksDb;
};

template <>
inline ECode Database::put(const std::experimental::string_view& key,
                           const std::string& value)
{
    EHTest(put(key, std::experimental::string_view(value)));
    EHEnd;
}

typedef std::shared_ptr<Database> DatabaseSPtr;

extern DatabaseSPtr gDatabase;
}
