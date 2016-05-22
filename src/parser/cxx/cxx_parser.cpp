//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/cxx/cxx_parser.h"
#include <iostream>
#include <axe.h>
#include <stddef.h>

namespace parser
{
using namespace axe;
static auto pound = r_char('#');
static auto include = r_str("include");
static auto endl = r_char('\n') | r_char('\r');
static auto space = r_char(' ') | r_char('\t');

static auto mlOpen = r_str("/*");
static auto mlClose = r_str("*/");
static auto mlComment = mlOpen & r_find(mlClose);

static auto slComment = r_str("//") & r_find(endl);

static auto codeLine = r_find(endl);

template <typename S>
void cxxFileParse(const S& store, const std::string& content)
{
    auto systemFile = r_find(r_char('>')) >> e_ref(store);
    auto systemHeader = r_char('<') & systemFile;
    auto programmerFile = r_find(r_char('"')) >> e_ref(store);
    auto programmerHeader = r_char('"') & programmerFile;
    auto header = systemHeader | programmerHeader;

    auto incLine = *space & pound & *space & include & *space & header &
                   (mlComment | slComment | (*space & (endl | r_end())));

    auto file = *(incLine | codeLine | mlComment);
    file(content.begin(), content.end());
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
