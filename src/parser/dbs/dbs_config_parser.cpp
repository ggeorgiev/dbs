//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "parser/dbs/e_particle.hpp"
#include "parser/dbs/e_position.hpp"
#include "parser/dbs/e_tag.hpp"
#include "parser/dbs/e_tag_expression.hpp"
#include "doim/tag/tag_expression.h"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>
#include <axe.h> // IWYU pragma: keep

namespace parser
{
static auto r_verboseKw = r_str("verbose");

ECode DbsConfigParser::parse(const doim::FsFileSPtr& dbsFile)
{
    // Config file
    const auto& path = dbsFile->path();
    if (!boost::filesystem::exists(path))
        EHBan(kFailed, "File not found", path);

    std::ifstream fstream(dbsFile->path(nullptr).c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    EHTest(parse(content));
    EHEnd;
}

ECode DbsConfigParser::parse(const string& content)
{
    // White space
    Position position;
    const auto& r_ws = position.r_ws();

    // Tag Expression
    TagSet tags;

    TagExpression tagExpression;

    auto r_verbose = r_ws & r_verboseKw & r_ws & r_colon & tagExpression.rule(r_ws) &
                     r_ws & r_semicolon;

    auto r_config = r_verbose;

    r_config(content.begin(), content.end());

    mTagExpression = tagExpression.mTagExpression;
    EHEnd;
}
}
