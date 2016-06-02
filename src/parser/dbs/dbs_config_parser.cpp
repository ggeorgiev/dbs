//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "doim/tag/tag_expression.h"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <axe.h>   // IWYU pragma: keep

namespace parser
{
using namespace axe;
static auto r_endl = r_str("\r\n") | r_str("\n\r") | r_char('\n');
static auto r_space = r_char(' ') | r_char('\t');

static auto r_colon = r_char(':');
static auto r_semicolon = r_char(';');

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
    typedef const string::const_iterator I;

    // White space
    size_t line;
    string::const_iterator lineIterator;

    auto newlineFn = [&line, &lineIterator](I& i1, I& i2) {
        lineIterator = i2;
        ++line;
    };

    auto r_ws = r_space | (r_endl >> e_ref(newlineFn));

    // Header end
    auto r_he = *r_ws & r_colon;

    // Structure end
    auto r_se = *r_ws & r_semicolon;

    // Tag Expression
    doim::TagSPtr tag;
    auto tagFn = [&tag](I& i1, I& i2) { tag = doim::Tag::unique(string(i1, i2)); };
    auto r_tag = *r_ws & r_ident() >> e_ref(tagFn);

    doim::TagSetSPtr tags;
    auto tagsInitFn = [&tags](I& i1, I& i2) { tags = doim::TagSet::make(); };

    auto tagsFn = [&tags, &tag](I& i1, I& i2) { tags->insert(tag); };
    auto r_tags = r_empty() >> e_ref(tagsInitFn) & +(r_tag >> e_ref(tagsFn));

    doim::TagExpression::ETurn turn;
    auto turnFn = [&turn](I& i1, I& i2) {
        turn = *i1 == '+' ? doim::TagExpression::ETurn::kOn
                          : doim::TagExpression::ETurn::kOff;
    };
    auto r_turn = *r_ws & r_turnChar >> e_ref(turnFn);

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

    auto r_verbose = *r_ws & r_verboseKeyword & r_he &
                     (+r_tagExpression >> e_ref(tagExpressionFn)) & r_se;

    auto r_config = r_verbose;

    r_config(content.begin(), content.end());

    mTagExpression = tagExpression;
    EHEnd;
}
}
