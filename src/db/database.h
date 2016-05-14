//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <rocksdb/db.h>
#include <memory>
#include <sstream>
#include <string_view>

namespace db
{
class Database
{
public:
    static constexpr int initialization_rank()
    {
        return im::InitializationManager::rank_base() +
               im::InitializationManager::rank_step();
    }

    ECode open(const std::string& file);
    void close();

    template <typename V>
    ECode get(const string_view& key, V& value)
    {
        V defaultValue = V();

        std::string buffer;
        EHTest(get(key,
                   string_view(reinterpret_cast<const char*>(&defaultValue),
                               sizeof(defaultValue)),
                   buffer));
        value = *reinterpret_cast<const V*>(buffer.data());
        EHEnd;
    }

    template <typename V>
    ECode put(const string_view& key, const V& value)
    {
        EHTest(
            put(key, string_view(reinterpret_cast<const char*>(&value), sizeof(value))));
        EHEnd;
    }

    ECode get(const string_view& key,
              const string_view& defaultValue,
              std::string& value);
    ECode get(const string_view& key, std::string& value);
    ECode put(const string_view& key, const string_view& value);
    ECode erase(const string_view& key);

private:
    std::unique_ptr<rocksdb::DB> mRocksDb;
};

template <>
inline ECode Database::put(const string_view& key, const std::string& value)
{
    EHTest(put(key, string_view(value)));
    EHEnd;
}

typedef shared_ptr<Database> DatabaseSPtr;

extern DatabaseSPtr gDatabase;
}
