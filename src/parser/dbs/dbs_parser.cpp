//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "doim/generic/object.h"
#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <iostream>
#include <axe.h>
#include <stddef.h>

namespace parser
{
using namespace axe;
static auto r_endl = r_str("\r\n") | r_str("\n\r") | r_char('\n');
static auto r_space = r_char(' ') | r_char('\t');

static auto r_colon = r_char(':');
static auto r_semicolon = r_char(';');
static auto r_slash = r_char('/');

static auto r_illegal = r_any("/?<>\\:;*|\"");
static auto r_pathChars = r_any() - r_illegal - r_space - r_endl;
static auto r_file = +r_pathChars & *(r_slash & +r_pathChars);

static auto r_annexKeyword = r_str("annex");

static auto r_at = r_char('@');
static auto r_equal = r_char('=');

static auto r_cxxLibraryKeyword = r_str("cxx_library");
static auto r_cxxLibraryCxxLibraryKeyword = r_str("cxx_library");
static auto r_cxxLibraryCxxHeaderKeyword = r_str("cxx_header");
static auto r_cxxLibraryCxxFileKeyword = r_str("cxx_file");

ECode DbsParser::parse(const doim::FsFileSPtr& dbsFile)
{
    typedef const string::const_iterator I;
    ILOG("Parse dbs file: {}", dbsFile->path());

    doim::FsDirectorySPtr location = dbsFile->directory();
    std::vector<string> errors;

    // White space
    size_t line;
    string::const_iterator lineIterator;

    auto newlineFn = [&line, &lineIterator](I& i1, I& i2) {
        lineIterator = i2;
        ++line;
    };

    auto r_wspace = r_space | (r_endl >> e_ref(newlineFn));

    // Attribute
    doim::AttributeNameSPtr attributeName;
    auto attributeNameFn = [&attributeName](I& i1, I& i2) {
        attributeName = doim::AttributeName::unique(string(i1, i2));
    };
    auto r_attributeName = r_ident() >> e_ref(attributeNameFn);

    doim::AttributeValueSPtr attributeValue;
    auto attributeValueFn = [&attributeValue](I& i1, I& i2) {
        attributeValue = doim::AttributeValue::unique(string(i1, i2));
    };
    auto r_attributeValue = r_ident() >> e_ref(attributeValueFn);

    doim::AttributeSPtr attribute;
    auto attributeFn = [&attributeName, &attributeValue, &attribute](I& i1, I& i2) {
        attribute = doim::Attribute::unique(attributeName, attributeValue);
    };

    auto r_attribute = (r_at & *r_wspace & r_attributeName & *r_wspace & r_equal &
                        *r_wspace & r_attributeValue) >>
                       e_ref(attributeFn);

    // Annex
    auto fileFn = [&location, &errors](I& i1, I& i2) {
        auto size = std::distance(i1, i2);
        auto annexPath = doim::FsFile::obtain(location, string_view(&*i1, size));

        DbsParser parser;
        ECode code = parser.parse(annexPath);
        if (code != err::kSuccess)
        {
            auto error = err::gError.release();
            errors.push_back(error->message());
        }
    };

    auto r_annex = r_annexKeyword & *r_wspace & r_colon & *r_wspace &
                   *(r_file >> e_ref(fileFn) & *r_wspace) & r_semicolon;

    // CxxLibrary
    dom::CxxLibrarySPtr cxxLibrary;

    /*
        // CxxLibrary CxxHeader

                doim::FsFileSetSPtr files;
                doim::FsDirectorySPtr directory;
                auto cxxHeaderFn = [=, &directory, &files]() {
                    // doim::AttributeSPtr visibility;
                    auto initFn = [&directory, &files](I& i1, I& i2) {
                        // visibility.reset();
                        directory.reset();
                        files = doim::FsFileSet::make();
                    };

                    auto attributeFn = [=, &directory](I& i1, I& i2) {
                        if (attribute->name() == dom::CxxLibrary::gVisibility)
                        {
                            // visibility = attribute;
                        }
                        else if (attribute->name() == dom::CxxLibrary::gDirectory)
                        {
                            directory =
                                doim::FsDirectory::obtain(location,
           attribute->value()->value());
                        }
                    };

                    auto fileFn = [directory, files](I& i1, I& i2) {
                        auto size = std::distance(i1, i2);
                        auto header = doim::FsFile::obtain(directory, string_view(&*i1,
           size));
                        files->insert(header);
                    };

                    return (r_cxxLibraryCxxHeaderKeyword >> e_sref(initFn)) & *r_wspace &
                           *(r_attribute >> e_sref(attributeFn)) & *r_wspace &
                           *(r_file >> e_sref(fileFn) & *r_wspace) & r_semicolon;
                };

                const auto& r_cxxHeader = cxxHeaderFn();
*/
    // static auto r_cxxLibraryCxxLibraryKeyword = r_str("cxx_library");

    // static auto r_cxxLibraryCxxFileKeyword = r_str("cxx_file");

    auto cxxLibraryNameFn = [&location, &cxxLibrary](I& i1, I& i2) {
        auto objType = doim::Object::EType::kCxxLibrary;
        const auto name = string(i1, i2);
        auto object = doim::Object::obtain(objType, location, name);
        cxxLibrary = dom::CxxLibrary::obtain(object);
    };

    auto r_cxxLibraryName = r_ident() >> e_ref(cxxLibraryNameFn);

    auto cxxLibraryAttributeFn = [&attribute, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary->updateAttribute(attribute);
    };

    auto r_cxxLibraryAttribute =
        (r_attribute >> e_ref(cxxLibraryAttributeFn)) & *r_wspace;

    auto r_cxxLibraryHeader = r_cxxLibraryKeyword & *r_wspace & r_cxxLibraryName &
                              *r_wspace & *r_cxxLibraryAttribute & r_colon;

    // return
    // & *r_wspace & *r_cxxHeader & *r_wspace & r_semicolon;

    auto r_cxxLibrary = r_cxxLibraryHeader & *r_wspace & r_semicolon;

    // Dbs file
    const auto& path = dbsFile->path();
    if (!boost::filesystem::exists(path))
        EHBan(kFailed, "File not found", path);

    std::ifstream fstream(dbsFile->path(nullptr).c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    const auto& r_dbs = ~r_annex & *(r_cxxLibrary & *r_wspace);

    r_dbs(content.begin(), content.end());

    for (const auto& error : errors)
        std::cout << error << "\n";
    EHEnd;
}
}
