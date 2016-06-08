//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <memory>
#include <str>
#include <tuple>
#include <vector>

namespace task
{
class CxxSourceHeadersTask;
typedef shared_ptr<CxxSourceHeadersTask> CxxSourceHeadersTaskSPtr;

struct CxxSourceHeadersTaskEnums
{
    enum class EDepth
    {
        kOne,
        kAll
    };
};

class CxxSourceHeadersTask : public CxxSourceHeadersTaskEnums,
                             public Element<CxxSourceHeadersTask,
                                            CxxSourceHeadersTaskEnums::EDepth,
                                            doim::CxxSourceSPtr,
                                            doim::CxxIncludeDirectorySPtr>
{
public:
    CxxSourceHeadersTask(const EDepth depth,
                         const doim::CxxSourceSPtr& cxxSource,
                         const doim::CxxIncludeDirectorySPtr& cxxIncludeDirectory);

    EDepth depth() const
    {
        return std::get<0>(mArgs);
    }

    const doim::CxxSourceSPtr& cxxSource() const
    {
        return std::get<1>(mArgs);
    }

    const doim::CxxIncludeDirectorySPtr& cxxIncludeDirectory() const
    {
        return std::get<2>(mArgs);
    }

    const std::vector<doim::CxxIncludeDirectory::CxxHeaderInfo>& headersInfo() const
    {
        return mHeadersInfo;
    }

    doim::TagSet tags() const override
    {
        return doim::TagSet{doim::gTaskTag, doim::gParseTag};
    }

    ECode operator()() override;
    string description() const override;

private:
    ECode one();

    std::vector<doim::CxxIncludeDirectory::CxxHeaderInfo> mHeadersInfo;
};
}
