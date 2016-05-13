//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "db/database.h"
#include "err/err.h"
#include "log/log.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <rocksdb/status.h>
#include <functional>
#include <sstream>

namespace db
{
DatabaseSPtr gDatabase = im::InitializationManager::subscribe(gDatabase);

ECode Database::open(const std::string& file)
{
    boost::filesystem::path path(file);
    if (!boost::filesystem::create_directories(path.parent_path()))
    {
        if (!boost::filesystem::exists(path.parent_path()))
            EHBan(kFileSystem, file);
    }

    rocksdb::Options options;
    options.create_if_missing = true;

    rocksdb::DB* db;
    const auto& status = rocksdb::DB::Open(options, file, &db);

    if (!status.ok())
    {
        const auto& msg = status.ToString();
        EHBan(kDatabase, msg);
    }

    mRocksDb.reset(db);
    EHEnd;
}

void Database::close()
{
    mRocksDb.reset();
}

ECode Database::get(const string_view& key,
                    const string_view& defaultValue,
                    std::string& value)
{
    const auto& status = mRocksDb->Get(rocksdb::ReadOptions(),
                                       rocksdb::Slice(key.data(), key.size()),
                                       &value);
    if (!status.ok())
    {
        if (status.code() == rocksdb::Status::kNotFound)
        {
            value = defaultValue.to_string();
            EHEnd;
        }

        const auto& msg = status.ToString();
        EHBan(kDatabase, msg);
    }
    DLOG("Read key: \"{}\", value: \"{}\"", key.to_string(), value);
    EHEnd;
}

ECode Database::get(const string_view& key, std::string& value)
{
    const auto& status = mRocksDb->Get(rocksdb::ReadOptions(),
                                       rocksdb::Slice(key.data(), key.size()),
                                       &value);
    if (!status.ok())
    {
        if (status.code() == rocksdb::Status::kNotFound)
            EHBan(kNotFound, key.to_string());

        const auto& msg = status.ToString();
        EHBan(kDatabase, msg);
    }

    DLOG("Read key: \"{}\", value: \"{}\"", key.to_string(), value);
    EHEnd;
}

ECode Database::put(const string_view& key, const string_view& value)
{
    const auto& status = mRocksDb->Put(rocksdb::WriteOptions(),
                                       rocksdb::Slice(key.data(), key.size()),
                                       rocksdb::Slice(value.data(), value.size()));
    if (!status.ok())
    {
        const auto& msg = status.ToString();
        EHBan(kDatabase, msg);
    }
    EHEnd;
}

ECode Database::erase(const string_view& key)
{
    const auto& status =
        mRocksDb->Delete(rocksdb::WriteOptions(), rocksdb::Slice(key.data(), key.size()));
    if (!status.ok())
    {
        const auto& msg = status.ToString();
        EHBan(kDatabase, msg);
    }
    EHEnd;
}

} // namespace db
