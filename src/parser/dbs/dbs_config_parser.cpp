//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "parser/dbs/e_particle.hpp"
#include "parser/dbs/e_position.hpp"
#include "parser/dbs/e_tag.hpp"
#include "parser/dbs/e_tag_expression.hpp"
#include "parser/dbs/e_tag_set.hpp"
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
    const auto& r_he = Particle::r_he(r_ws);
    const auto& r_se = Particle::r_se(r_ws);

    // Tag Expression
    Tag tag;
    auto r_tag = r_ws & r_ident() >> e_ref(tag);

    TagSet tags(tag);
    auto r_tags = r_empty() >> tags.reset() & +(r_tag >> e_ref(tags));

    TagExpression tagExpression(tags);
    auto r_tagExpression =
        r_ws & r_turnChar >> tagExpression.turn() & +(r_tags >> tagExpression.section());

    auto r_verbose =
        r_ws & r_verboseKw & r_he & +r_tagExpression >> e_ref(tagExpression) & r_se;

    auto r_config = r_verbose;

    r_config(content.begin(), content.end());

    mTagExpression = tagExpression.mTagExpression;
    EHEnd;
}
}
