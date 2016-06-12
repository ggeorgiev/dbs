//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/crc_task.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <boost/hana/for_each.hpp>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxSourceCrcTask;
typedef shared_ptr<CxxSourceCrcTask> CxxSourceCrcTaskSPtr;

struct CxxSourceCrcTaskEnums
{
    enum class EDepth
    {
        kOne,
        kAll
    };
};

class CxxSourceCrcTask : public CxxSourceCrcTaskEnums,
                         public CrcTask<CxxSourceCrcTask,
                                        CxxSourceCrcTaskEnums::EDepth,
                                        doim::CxxSourceSPtr,
                                        doim::CxxIncludeDirectorySPtr>
{
public:
    using CrcTask<CxxSourceCrcTask,
                  CxxSourceCrcTaskEnums::EDepth,
                  doim::CxxSourceSPtr,
                  doim::CxxIncludeDirectorySPtr>::CrcTask;
    bool check() const;

    EDepth depth() const
    {
        return std::get<0>(mArgs);
    }

    const doim::CxxSourceSPtr& cxxSource() const
    {
        return std::get<1>(mArgs);
    }

    doim::CxxIncludeDirectorySPtr currentIncludeDirectory() const
    {
        return std::get<2>(mArgs);
    }

    doim::TagSetSPtr tags() const override;
    ECode operator()() override;
    string description() const override;

private:
    ECode one();
};
}
