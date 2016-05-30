//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "task/cxx/cxx_source_headers_task.h"
#include "parser/cxx/cxx_parser.h"
#include "doim/cxx/cxx_header.h" // IWYU pragma: keep
#include "err/err_assert.h"
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>
#include <string_view>

namespace task
{
CxxSourceHeadersTask::CxxSourceHeadersTask(
    const CxxSourceSPtr& cxxSource,
    const doim::CxxIncludeDirectorySPtr& cxxIncludeDirectory)
    : Element(cxxSource, cxxIncludeDirectory)
{
    ASSERT(boost::apply_visitor([](auto const& source) { return source->isUnique(); },
                                cxxSource));
}

ECode CxxSourceHeadersTask::operator()()
{
    const auto& path =
        boost::apply_visitor([](auto const& source) { return source->file()->path(); },
                             cxxSource());

    std::ifstream fstream(path.c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    const auto& includeDirectories = boost::apply_visitor(
        [](auto const& source) { return source->cxxIncludeDirectories(); }, cxxSource());

    parser::CxxParser parser;

    for (const auto& include : parser.includes(content))
    {
        auto currentDirectory =
            include.mType == parser::CxxParser::EIncludeType::kProgrammer
                ? cxxIncludeDirectory()
                : nullptr;
        doim::CxxIncludeDirectory::CxxHeaderInfo headerInfo;
        EHTest(doim::CxxIncludeDirectory::findHeader(string_view(include.mPath),
                                                     currentDirectory,
                                                     includeDirectories,
                                                     headerInfo),
               path);
        mHeadersInfo.push_back(headerInfo);
    }
    EHEnd;
}

string CxxSourceHeadersTask::description() const
{
    auto path =
        boost::apply_visitor([](auto const& source) { return source->file()->path(); },
                             cxxSource());
    return "Cxx file headers " + path;
}

} // namespace task
