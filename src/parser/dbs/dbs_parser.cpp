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
static auto r_path = +r_pathChars & *(r_slash & +r_pathChars);

static auto r_annexKeyword = r_str("annex");

static auto r_at = r_char('@');
static auto r_equal = r_char('=');

static auto r_cxxLibraryKeyword = r_str("cxx_library");
static auto r_cxxLibraryCxxLibraryKeyword = r_str("cxx_library");
static auto r_cxxLibraryCxxHeaderKeyword = r_str("cxx_header");
static auto r_cxxLibraryCxxFileKeyword = r_str("cxx_file");
// TODO: change this ti just binary
static auto r_cxxLibraryCxxBinaryKeyword = r_str("cxx_binary");

ECode DbsParser::parse(const doim::FsFileSPtr& dbsFile)
{
    typedef const string::const_iterator I;
    // DLOG("Parse dbs file: {}", dbsFile->path());

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

    // File
    doim::FsFileSPtr file;
    doim::FsDirectorySPtr directory;
    auto fileSetLocationFn = [&location, &directory](I& i1, I& i2) {
        directory = location;
    };
    auto r_setLocation = r_empty() >> e_ref(fileSetLocationFn);
    auto fileFn = [&directory, &file](I& i1, I& i2) {
        auto size = std::distance(i1, i2);
        file = doim::FsFile::obtain(directory, string_view(&*i1, size));
    };
    auto r_file = r_path >> e_ref(fileFn);

    // Files
    doim::FsFileSet files;
    auto filesResetFn = [&files](I& i1, I& i2) { files.clear(); };
    auto filesFn = [&file, &files](I& i1, I& i2) { files.insert(file); };
    auto r_files =
        (r_empty() >> e_ref(filesResetFn)) & *((r_file >> e_ref(filesFn)) & *r_wspace);

    // Object
    doim::ObjectSPtr object;
    doim::Object::EType objType;
    auto objTypeSetCxxLibraryFn = [&objType](I& i1, I& i2) {
        objType = doim::Object::EType::kCxxLibrary;
    };
    auto r_setObjTypeCxxLibrary = r_empty() >> e_ref(objTypeSetCxxLibraryFn);

    auto objectFn = [&location, objType, &object](I& i1, I& i2) {
        auto size = std::distance(i1, i2);
        auto path = string_view(&*i1, std::distance(i1, i2));
        object = doim::Object::obtain(objType, location, path);
    };
    auto r_object = r_path >> e_ref(objectFn);

    // dom::CxxLibrary Object
    dom::CxxLibrarySPtr cxxLibraryObject;
    auto cxxLibraryObjectFn = [&object, &cxxLibraryObject](I& i1, I& i2) {
        cxxLibraryObject = dom::CxxLibrary::obtain(object);
    };
    auto r_cxxLibraryObject =
        r_setObjTypeCxxLibrary & (r_object >> e_ref(cxxLibraryObjectFn));

    // Annex
    auto annexFileFn = [&file, &errors](I& i1, I& i2) {
        DbsParser parser;
        ECode code = parser.parse(file);
        if (code != err::kSuccess)
        {
            auto error = err::gError.release();
            errors.push_back(error->message());
        }
    };

    auto r_annex = r_annexKeyword & *r_wspace & r_colon & *r_wspace & r_setLocation &
                   *(r_file >> e_ref(annexFileFn) & *r_wspace) & r_semicolon;

    // CxxLibrary
    dom::CxxLibrarySPtr cxxLibrary;

    // CxxLibrary CxxHeader

    doim::AttributeSPtr cxxLibraryCxxHeaderVisibility;
    auto cxxLibraryCxxHeaderInitFn =
        [&location, &directory, &cxxLibraryCxxHeaderVisibility](I& i1, I& i2) {
            directory = location;
            cxxLibraryCxxHeaderVisibility.reset();
        };

    auto cxxLibraryCxxHeaderAttributeFn = [&location,
                                           &attribute,
                                           &directory,
                                           &cxxLibraryCxxHeaderVisibility](I& i1, I& i2) {
        if (attribute->name() == dom::CxxLibrary::gVisibility)
            cxxLibraryCxxHeaderVisibility = attribute;
        else if (attribute->name() == dom::CxxLibrary::gDirectory)
            directory = doim::FsDirectory::obtain(location, attribute->value()->value());
    };

    auto r_cxxLibraryCxxHeaderAttribute =
        r_attribute >> e_ref(cxxLibraryCxxHeaderAttributeFn);

    auto r_cxxLibraryCxxHeaderCat =
        (r_cxxLibraryCxxHeaderKeyword >> e_ref(cxxLibraryCxxHeaderInitFn)) & *r_wspace &
        *r_cxxLibraryCxxHeaderAttribute & *r_wspace & r_colon;

    auto cxxLibraryCxxHeaderFn =
        [&cxxLibrary, &directory, &files, &cxxLibraryCxxHeaderVisibility](I& i1, I& i2) {

            if (cxxLibraryCxxHeaderVisibility != nullptr &&
                cxxLibraryCxxHeaderVisibility->value() == dom::CxxLibrary::gPublic)
                cxxLibrary->updateCxxPublicHeaders(directory, files);
            else
                cxxLibrary->updateCxxPublicHeaders(directory, files);
        };

    auto r_cxxLibraryCxxHeader =
        (r_cxxLibraryCxxHeaderCat & *r_wspace & r_files & r_semicolon) >>
        e_ref(cxxLibraryCxxHeaderFn);

    // CxxLibrary CxxLibrary

    dom::CxxLibrarySet cxxLibraryCxxLibraryLibraries;
    auto cxxLibraryCxxLibraryInitFn = [&cxxLibraryCxxLibraryLibraries](I& i1, I& i2) {
        cxxLibraryCxxLibraryLibraries.clear();
    };

    auto cxxLibraryCxxLibraryLibraryFn = [&cxxLibraryObject,
                                          &cxxLibraryCxxLibraryLibraries](I& i1, I& i2) {
        cxxLibraryCxxLibraryLibraries.insert(cxxLibraryObject);
    };
    auto r_cxxLibraryCxxLibraryLibrary =
        r_cxxLibraryObject >> e_ref(cxxLibraryCxxLibraryLibraryFn);

    auto r_cxxLibraryCxxLibraryCat =
        (r_cxxLibraryCxxLibraryKeyword >> e_ref(cxxLibraryCxxLibraryInitFn)) & *r_wspace &
        r_colon;

    auto cxxLibraryCxxLibraryFn = [&cxxLibrary, &cxxLibraryCxxLibraryLibraries](I& i1,
                                                                                I& i2) {
        cxxLibrary->updateCxxLibraries(cxxLibraryCxxLibraryLibraries);
    };

    auto r_cxxLibraryCxxLibrary =
        (r_cxxLibraryCxxLibraryCat & *r_wspace &
         *(r_cxxLibraryCxxLibraryLibrary & *r_wspace) & r_semicolon) >>
        e_ref(cxxLibraryCxxLibraryFn);

    // CxxLibrary CxxFile
    auto r_cxxLibraryCxxFileCat = r_cxxLibraryCxxFileKeyword & *r_wspace & r_colon;

    auto cxxLibraryCxxFileFn = [&cxxLibrary, &files](I& i1, I& i2) {
        cxxLibrary->updateCxxFilesList(files);
    };

    auto r_cxxLibraryCxxFile =
        (r_cxxLibraryCxxFileCat & *r_wspace & r_setLocation & r_files & r_semicolon) >>
        e_ref(cxxLibraryCxxFileFn);

    // CxxLibrary CxxBinary
    auto cxxLibraryCxxBinaryFn = [&file, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary->updateBinary(file);
    };
    auto r_cxxLibraryCxxBinary = r_cxxLibraryCxxBinaryKeyword & *r_wspace & r_colon &
                                 *r_wspace & r_setLocation &
                                 (r_file >> e_ref(cxxLibraryCxxBinaryFn)) & r_semicolon;

    // ... CxxLibrary
    auto cxxLibraryNameFn = [&cxxLibraryObject, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary = cxxLibraryObject;
    };
    auto r_cxxLibraryName = r_cxxLibraryObject >> e_ref(cxxLibraryNameFn);

    auto cxxLibraryAttributeFn = [&attribute, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary->updateAttribute(attribute);
    };

    auto r_cxxLibraryAttribute =
        (r_attribute >> e_ref(cxxLibraryAttributeFn)) & *r_wspace;

    auto r_cxxLibraryCap = r_cxxLibraryKeyword & *r_wspace & r_cxxLibraryName &
                           *r_wspace & *r_cxxLibraryAttribute & r_colon;

    auto r_cxxLibrary = r_cxxLibraryCap & *r_wspace &
                        *((r_cxxLibraryCxxHeader | r_cxxLibraryCxxLibrary |
                           r_cxxLibraryCxxFile | r_cxxLibraryCxxBinary) &
                          *r_wspace) &
                        r_semicolon;

    // Dbs file
    const auto& path = dbsFile->path();
    if (!boost::filesystem::exists(path))
        EHBan(kFailed, "File not found", path);

    std::ifstream fstream(dbsFile->path(nullptr).c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    const auto& r_dbs = ~r_annex & *(r_cxxLibrary & *r_wspace);

    r_dbs(content.begin(), content.end());

    // for (const auto& error : errors)
    //    std::cout << error << "\n";
    EHEnd;
}
}
