//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element.hpp"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/tag/tag.h"
#include "err/err.h"
#include <fmt/format.h>
#include <boost/variant/variant.hpp>
#include <memory>
#include <str>
#include <tuple>
#include <vector>

namespace task
{
class CxxSourceHeadersTask;
typedef shared_ptr<CxxSourceHeadersTask> CxxSourceHeadersTaskSPtr;

struct CxxSourceHeadersTaskVariants
{
    typedef boost::variant<doim::CxxFileSPtr, doim::CxxHeaderSPtr> CxxSourceSPtr;
};

class CxxSourceHeadersTask : public CxxSourceHeadersTaskVariants,
                             public Element<CxxSourceHeadersTask,
                                            CxxSourceHeadersTaskVariants::CxxSourceSPtr,
                                            doim::CxxIncludeDirectorySPtr>
{
public:
    CxxSourceHeadersTask(const CxxSourceSPtr& cxxSource,
                         const doim::CxxIncludeDirectorySPtr& cxxIncludeDirectory);

    const CxxSourceSPtr& cxxSource() const
    {
        return std::get<0>(mArgs);
    }

    const doim::CxxIncludeDirectorySPtr& cxxIncludeDirectory() const
    {
        return std::get<1>(mArgs);
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
    std::vector<doim::CxxIncludeDirectory::CxxHeaderInfo> mHeadersInfo;
};
}
