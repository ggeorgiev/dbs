//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "parser/axe.hpp"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
#include "doim/fs/fs_directory.h"
#include "doim/generic/attribute.h"
#include "doim/generic/attribute_name.h"
#include "doim/generic/attribute_value.h"
#include "doim/generic/object.h"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <memory>
#include <str>
#include <string_view>
#include <vector>
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
static auto r_path = ~r_slash & +r_pathChars & *(r_slash & +r_pathChars) & ~r_slash;

static auto r_annexKeyword = r_str("annex");

static auto r_at = r_char('@');
static auto r_equal = r_char('=');

static auto r_cxxProgramKeyword = r_str("cxx_program");

static auto r_cxxLibraryKeyword = r_str("cxx_library");
static auto r_cxxFileKeyword = r_str("cxx_file");

static auto r_cxxLibraryCxxHeaderKeyword = r_str("cxx_header");
static auto r_cxxLibraryBinaryKeyword = r_str("binary");

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

    auto r_ws = r_space | (r_endl >> e_ref(newlineFn));
    // Header end
    auto r_he = *r_ws & r_colon;
    // Structure end
    auto r_se = *r_ws & r_semicolon;

    // Attribute
    doim::AttributeNameSPtr attributeName;
    auto attributeNameFn = [&attributeName](I& i1, I& i2) {
        attributeName = doim::AttributeName::unique(string(i1, i2));
    };
    auto r_attributeName = *r_ws & r_ident() >> e_ref(attributeNameFn);

    doim::AttributeValueSPtr attributeValue;
    auto attributeValueFn = [&attributeValue](I& i1, I& i2) {
        attributeValue = doim::AttributeValue::unique(string(i1, i2));
    };
    auto r_attributeValue = *r_ws & (r_ident() | r_path) >> e_ref(attributeValueFn);

    doim::AttributeSPtr attribute;
    auto attributeFn = [&attributeName, &attributeValue, &attribute](I& i1, I& i2) {
        attribute = doim::Attribute::unique(attributeName, attributeValue);
    };
    auto r_attribute =
        (*r_ws & r_at & r_attributeName & *r_ws & r_equal & r_attributeValue) >>
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
    auto r_file = *r_ws & r_path >> e_ref(fileFn);

    // Files
    doim::FsFileSet files;
    auto filesResetFn = [&files](I& i1, I& i2) { files.clear(); };
    auto filesFn = [&file, &files](I& i1, I& i2) { files.insert(file); };
    auto r_files = (r_empty() >> e_ref(filesResetFn)) & *(r_file >> e_ref(filesFn));

    // Object
    doim::ObjectSPtr object;
    doim::Object::EType objType;
    auto objectFn = [&location, &objType, &object](I& i1, I& i2) {
        auto size = std::distance(i1, i2);
        auto path = string_view(&*i1, std::distance(i1, i2));
        object = doim::Object::obtain(objType, location, path);
    };
    auto r_object = *r_ws & r_path >> e_ref(objectFn);

    auto objTypeSetCxxLibraryFn = [&objType](I& i1, I& i2) {
        objType = doim::Object::EType::kCxxLibrary;
    };
    auto r_objectCxxLibrary = r_empty() >> e_ref(objTypeSetCxxLibraryFn) & r_object;

    auto objTypeSetCxxProgramFn = [&objType](I& i1, I& i2) {
        objType = doim::Object::EType::kCxxProgram;
    };
    auto r_objectCxxProgram = r_empty() >> e_ref(objTypeSetCxxProgramFn) & r_object;

    // dom::CxxLibrary Object
    dom::CxxLibrarySPtr cxxLibraryObject;
    auto cxxLibraryObjectFn = [&object, &cxxLibraryObject](I& i1, I& i2) {
        cxxLibraryObject = dom::CxxLibrary::obtain(object);
    };
    auto r_cxxLibraryObject = r_objectCxxLibrary >> e_ref(cxxLibraryObjectFn);

    // CxxFiles
    auto r_cxxFiles = *r_ws & r_cxxFileKeyword & r_he & r_setLocation & r_files & r_se;

    // CxxLibraries

    dom::CxxLibrarySet cxxLibraries;
    auto cxxLibrariesInitFn = [&cxxLibraries](I& i1, I& i2) { cxxLibraries.clear(); };

    auto cxxLibraryFn = [&cxxLibraryObject, &cxxLibraries](I& i1, I& i2) {
        cxxLibraries.insert(cxxLibraryObject);
    };
    auto r_cxxLibraryItem = r_cxxLibraryObject >> e_ref(cxxLibraryFn);

    auto r_cxxLibrariesCat =
        *r_ws & r_cxxLibraryKeyword >> e_ref(cxxLibrariesInitFn) & r_he;

    auto r_cxxLibraries = r_cxxLibrariesCat & *r_cxxLibraryItem & r_se;

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

    auto r_annex = *r_ws & r_annexKeyword & r_he & r_setLocation &
                   *(r_file >> e_ref(annexFileFn)) & r_se;

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
        *r_ws & (r_cxxLibraryCxxHeaderKeyword >> e_ref(cxxLibraryCxxHeaderInitFn)) &
        *r_cxxLibraryCxxHeaderAttribute & r_he;

    auto cxxLibraryCxxHeaderFn =
        [&cxxLibrary, &directory, &files, &cxxLibraryCxxHeaderVisibility](I& i1, I& i2) {
            if (cxxLibraryCxxHeaderVisibility != nullptr &&
                cxxLibraryCxxHeaderVisibility->value() == dom::CxxLibrary::gPublic)
                cxxLibrary->updateCxxPublicHeaders(directory, files);
            else
                cxxLibrary->updateCxxPrivateHeaders(directory, files);
        };

    auto r_cxxLibraryCxxHeader =
        (r_cxxLibraryCxxHeaderCat & r_files & r_se) >> e_ref(cxxLibraryCxxHeaderFn);

    // CxxLibrary CxxLibrary
    auto cxxLibraryCxxLibraryFn = [&cxxLibrary, &cxxLibraries](I& i1, I& i2) {
        cxxLibrary->updateCxxLibraries(cxxLibraries);
    };
    auto r_cxxLibraryCxxLibrary = r_cxxLibraries >> e_ref(cxxLibraryCxxLibraryFn);

    // CxxLibrary CxxFile
    auto cxxLibraryCxxFileFn = [&cxxLibrary, &files](I& i1, I& i2) {
        cxxLibrary->updateCxxFilesList(files);
    };
    auto r_cxxLibraryCxxFile = r_cxxFiles >> e_ref(cxxLibraryCxxFileFn);

    // CxxLibrary CxxBinary
    auto cxxLibraryCxxBinaryFn = [&file, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary->updateBinary(file);
    };
    auto r_cxxLibraryCxxBinary = *r_ws & r_cxxLibraryBinaryKeyword & r_he &
                                 r_setLocation &
                                 (r_file >> e_ref(cxxLibraryCxxBinaryFn)) & r_se;

    // ... CxxLibrary
    auto cxxLibraryNameFn = [&cxxLibraryObject, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary = cxxLibraryObject;
    };
    auto r_cxxLibraryName = r_cxxLibraryObject >> e_ref(cxxLibraryNameFn);

    auto cxxLibraryAttributeFn = [&attribute, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary->updateAttribute(attribute);
    };

    auto r_cxxLibraryAttribute = r_attribute >> e_ref(cxxLibraryAttributeFn);

    auto r_cxxLibraryCap =
        *r_ws & r_cxxLibraryKeyword & r_cxxLibraryName & *r_cxxLibraryAttribute & r_he;

    auto r_cxxLibrary = r_cxxLibraryCap &
                        *(r_cxxLibraryCxxFile | r_cxxLibraryCxxHeader |
                          r_cxxLibraryCxxLibrary | r_cxxLibraryCxxBinary) &
                        r_se;

    // CxxProgram
    dom::CxxProgramSPtr cxxProgram;

    // CxxProgram CxxLibrary
    auto cxxProgramCxxLibraryFn = [&cxxProgram, &cxxLibraries](I& i1, I& i2) {
        cxxProgram->updateCxxLibraries(cxxLibraries);
    };
    auto r_cxxProgramCxxLibrary = r_cxxLibraries >> e_ref(cxxProgramCxxLibraryFn);

    // CxxProgram CxxFile
    auto cxxProgramCxxFileFn = [&cxxProgram, &files](I& i1, I& i2) {
        cxxProgram->updateCxxFilesList(files);
    };
    auto r_cxxProgramCxxFile = r_cxxFiles >> e_ref(cxxProgramCxxFileFn);

    // ... CxxProgram
    auto cxxProgramNameFn = [&object, &cxxProgram](I& i1, I& i2) {
        cxxProgram = dom::CxxProgram::obtain(object);
        cxxProgram->updateName(object->name());
    };
    auto r_cxxProgramName = r_objectCxxProgram >> e_ref(cxxProgramNameFn);

    auto r_cxxProgramCap = *r_ws & r_cxxProgramKeyword & r_cxxProgramName & r_he;

    auto r_cxxProgram =
        r_cxxProgramCap & *(r_cxxProgramCxxFile | r_cxxProgramCxxLibrary) & r_se;

    // Dbs file
    const auto& path = dbsFile->path();
    if (!boost::filesystem::exists(path))
        EHBan(kFailed, "File not found", path);

    std::ifstream fstream(dbsFile->path(nullptr).c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    const auto& r_dbs = ~r_annex & *(r_cxxLibrary | r_cxxProgram);

    r_dbs(content.begin(), content.end());

    // for (const auto& error : errors)
    //    std::cout << error << "\n";
    EHEnd;
}
}
