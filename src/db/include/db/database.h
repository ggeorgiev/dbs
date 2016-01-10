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

    template <typename K, typename V>
    ECode get(const std::shared_ptr<K>& object, V& value)
    {
        const auto& key = object->key();
        V dflt = V();

        std::string buffer;
        EHTest(get(key,
                   std::experimental::string_view(reinterpret_cast<const char*>(&dflt),
                                                  sizeof(dflt)),
                   buffer));
        value = *reinterpret_cast<const V*>(buffer.data());
        EHEnd;
    }

    template <typename K, typename V>
    ECode put(const std::shared_ptr<K>& object, const V& value)
    {
        const auto& key = object->key();
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

typedef std::shared_ptr<Database> DatabaseSPtr;

extern DatabaseSPtr gDatabase;
}
