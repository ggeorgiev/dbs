//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "db/database.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <rocksdb/options.h>
#include <rocksdb/status.h>
#include <functional>
#include <sstream>

#include "err/err.h"
#include "err/err_cppformat.h"

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

} // namespace db
