//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h"
#include <type_traits>
#include <axe.h> // IWYU pragma: keep

namespace parser
{
using namespace axe;
static auto r_pound = r_char('#');
static auto r_include = r_str("include");
static auto r_endl = r_char('\n') | r_char('\r');
static auto r_space = r_char(' ') | r_char('\t');

static auto r_mlOpen = r_str("/*");
static auto r_mlClose = r_str("*/");
static auto r_mlComment = r_mlOpen & r_find(r_mlClose);

static auto r_slComment = r_str("//") & r_find(r_endl);

static auto r_codeLine = r_find(r_endl);

template <typename S>
void cxxFileParse(const S& store, const std::string& content)
{
    auto r_systemFile = r_find(r_char('>')) >> e_ref(store);
    auto r_systemHeader = r_char('<') & r_systemFile;
    auto r_programmerFile = r_find(r_char('"')) >> e_ref(store);
    auto r_programmerHeader = r_char('"') & r_programmerFile;
    auto r_header = r_systemHeader | r_programmerHeader;

    auto r_incLine = *r_space & r_pound & *r_space & r_include & *r_space & r_header &
                     (r_mlComment | r_slComment | (*r_space & (r_endl | r_end())));

    auto r_file = *(r_incLine | r_codeLine | r_mlComment);
    r_file(content.begin(), content.end());
}

std::vector<CxxParser::Include> CxxParser::includes(const string& content)
{
    std::vector<Include> result;

    auto store = [&result](const string::const_iterator& i1,
                           const string::const_iterator& i2) {
        auto e = i2 - 1;
        auto type = *e == '"' ? EIncludeType::kProgrammer : EIncludeType::kStandard;
        result.push_back(Include(type, std::string(i1, e)));
    };

    cxxFileParse(store, content);
    return result;
}
}
