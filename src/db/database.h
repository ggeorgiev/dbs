//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/tag/tag.h"
#include "err/err.h"
#include "im/initialization_manager.hpp"
#include <rocksdb/db.h>
#include <memory>
#include <str>
#include <string_view>

namespace db
{
class Database
{
public:
    static doim::TagSetSPtr gDbLoadSet;
    static doim::TagSetSPtr gDbSaveSet;

    static constexpr int rank()
    {
        return im::InitializationManager::rank() + im::InitializationManager::step();
    }

    ECode open(const string& file);
    void close();

    template <typename V>
    ECode get(const string_view& key, V& value)
    {
        V defaultValue = V();

        string buffer;
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

    ECode get(const string_view& key, const string_view& defaultValue, string& value);
    ECode get(const string_view& key, string& value);
    ECode put(const string_view& key, const string_view& value);
    ECode erase(const string_view& key);

private:
    std::unique_ptr<rocksdb::DB> mRocksDb;
};

template <>
inline ECode Database::put(const string_view& key, const string& value)
{
    EHTest(put(key, string_view(value)));
    EHEnd;
}

typedef shared_ptr<Database> DatabaseSPtr;

extern DatabaseSPtr gDatabase;
}
