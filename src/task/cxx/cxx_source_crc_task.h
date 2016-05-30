//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/crc_task.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <boost/variant/variant.hpp>
#include <memory>
#include <str>
#include <tuple>

namespace task
{
class CxxSourceCrcTask;
typedef shared_ptr<CxxSourceCrcTask> CxxSourceCrcTaskSPtr;

struct CxxSourceCrcTaskVariants
{
    typedef boost::variant<doim::CxxFileSPtr, doim::CxxHeaderSPtr> CxxSourceSPtr;
};

class CxxSourceCrcTask : public CxxSourceCrcTaskVariants,
                         public CrcTask<CxxSourceCrcTask,
                                        CxxSourceCrcTaskVariants::CxxSourceSPtr,
                                        doim::CxxIncludeDirectorySPtr>
{
public:
    CxxSourceCrcTask(const CxxSourceSPtr& cxxSource,
                     const doim::CxxIncludeDirectorySPtr& currentIncludeDirectory);

    const CxxSourceSPtr& cxxSource() const
    {
        return std::get<0>(mArgs);
    }

    doim::CxxIncludeDirectorySPtr currentIncludeDirectory() const
    {
        return std::get<1>(mArgs);
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gCrcTag};
    }

    ECode operator()() override;
    string description() const override;
};
}
