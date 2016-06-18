//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "parser/dbs/e_particle.hpp"
#include "parser/dbs/e_position.hpp"
#include "doim/tag/tag.h"
#include "doim/tag/tag_expression.h"
#include "doim/set.hpp"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <str>
#include <utility>
#include <vector>
#include <axe.h> // IWYU pragma: keep
#include <stddef.h>

namespace parser
{
static auto r_verboseKeyword = r_str("verbose");
static auto r_turnChar = r_char('+') | r_char('-');

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
    doim::TagSPtr tag;
    auto tagFn = [&tag](I& i1, I& i2) { tag = doim::Tag::unique(string(i1, i2)); };
    auto r_tag = r_ws & r_ident() >> e_ref(tagFn);

    doim::TagSetSPtr tags;
    auto tagsInitFn = [&tags](I& i1, I& i2) { tags = doim::TagSet::make(); };

    auto tagsFn = [&tags, &tag](I& i1, I& i2) { tags->insert(tag); };
    auto r_tags = r_empty() >> e_ref(tagsInitFn) & +(r_tag >> e_ref(tagsFn));

    doim::TagExpression::ETurn turn;
    auto turnFn = [&turn](I& i1, I& i2) {
        turn = *i1 == '+' ? doim::TagExpression::ETurn::kOn
                          : doim::TagExpression::ETurn::kOff;
    };
    auto r_turn = r_ws & r_turnChar >> e_ref(turnFn);

    std::vector<std::pair<doim::TagExpression::ETurn, doim::TagSetSPtr>> tagsSection;
    auto tagsSectionFn = [&turn, &tags, &tagsSection](I& i1, I& i2) {
        tagsSection.push_back(std::make_pair(turn, doim::TagSet::unique(tags)));
    };

    doim::TagExpressionSPtr tagExpression;
    auto tagExpressionFn = [&tagExpression, &tagsSection](I& i1, I& i2) {
        tagExpression.reset();
        for (auto it = tagsSection.rbegin(); it != tagsSection.rend(); ++it)
        {
            tagExpression =
                doim::TagExpression::unique(it->first, it->second, tagExpression);
        }
    };

    auto r_tagExpression = r_turn & +(r_tags >> e_ref(tagsSectionFn));

    auto r_verbose = r_ws & r_verboseKeyword & r_he &
                     (+r_tagExpression >> e_ref(tagExpressionFn)) & r_se;

    auto r_config = r_verbose;

    r_config(content.begin(), content.end());

    mTagExpression = tagExpression;
    EHEnd;
}
}
