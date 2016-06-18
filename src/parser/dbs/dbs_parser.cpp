//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
#include "dom/protobuf/protobuf_plugin.h"
#include "doim/cxx/cxx_header.h"
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
#include <axe.h> // IWYU pragma: keep
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

static auto r_protobufPluginKeyword = r_str("protobuf_plugin");

static auto r_cxxProgramKeyword = r_str("cxx_program");

static auto r_cxxLibraryKeyword = r_str("cxx_library");
static auto r_cxxFileKeyword = r_str("cxx_file");
static auto r_protobufFileKeyword = r_str("protobuf_file");

static auto r_cxxHeaderKeyword = r_str("cxx_header");
static auto r_binaryKeyword = r_str("binary");

typedef const string::const_iterator I;

struct Position
{
    void operator()(I& i1, I& i2)
    {
        lineIterator = i2;
        ++line;
    }

    size_t line;
    string::const_iterator lineIterator;
};

struct Attribute
{
    auto name()
    {
        return
            [this](I& i1, I& i2) { mName = doim::AttributeName::unique(string(i1, i2)); };
    }

    auto value()
    {
        return [this](I& i1, I& i2) {
            mValue = doim::AttributeValue::unique(string(i1, i2));
        };
    }

    void operator()(I& i1, I& i2)
    {
        mAttribute = doim::Attribute::unique(mName, mValue);
    }

    doim::AttributeNameSPtr mName;
    doim::AttributeValueSPtr mValue;
    doim::AttributeSPtr mAttribute;
};

struct Directory
{
    Directory(const doim::FsDirectorySPtr& directory)
        : mDefaultDirectory(directory)
    {
    }

    auto reset()
    {
        return [this](I& i1, I& i2) { mDirectory = mDefaultDirectory; };
    }

    doim::FsDirectorySPtr mDirectory;
    doim::FsDirectorySPtr mDefaultDirectory;
};

struct File
{
    File(Directory& directory)
        : mDirectory(directory)
    {
    }

    void operator()(I& i1, I& i2)
    {
        auto size = std::distance(i1, i2);
        mFile = doim::FsFile::obtain(mDirectory.mDirectory, string_view(&*i1, size));
    };

    doim::FsFileSPtr mFile;
    Directory& mDirectory;
};

struct FileSet
{
    FileSet(File& file)
        : mFile(file)
    {
    }

    auto reset()
    {
        return [this](I& i1, I& i2) { mFiles.clear(); };
    }

    void operator()(I& i1, I& i2)
    {
        mFiles.insert(mFile.mFile);
    };

    doim::FsFileSet mFiles;
    File& mFile;
};

struct Object
{
    Object(const doim::FsDirectorySPtr& location)
        : mLocation(location)
    {
    }

    auto cxxLibrary()
    {
        return [this](I& i1, I& i2) { mObjType = doim::Object::EType::kCxxLibrary; };
    }

    auto cxxProgram()
    {
        return [this](I& i1, I& i2) { mObjType = doim::Object::EType::kCxxProgram; };
    }

    void operator()(I& i1, I& i2)
    {
        auto size = std::distance(i1, i2);
        auto path = string_view(&*i1, std::distance(i1, i2));
        mObject = doim::Object::obtain(mObjType, mLocation, path);
    }

    doim::FsDirectorySPtr mLocation;
    doim::ObjectSPtr mObject;
    doim::Object::EType mObjType;
};

struct Annex
{
    Annex(std::vector<string>& errors, File& file)
        : mErrors(errors)
        , mFile(file)
    {
    }

    void operator()(I& i1, I& i2)
    {
        DbsParser parser;
        ECode code = parser.parse(mFile.mFile);
        if (code != err::kSuccess)
        {
            auto error = err::gError.release();
            mErrors.push_back(error->message());
        }
    };

    File& mFile;
    std::vector<string>& mErrors;
};

struct CxxLibraryRef
{
    CxxLibraryRef(Object& object)
        : mObject(object)
    {
    }

    void operator()(I& i1, I& i2)
    {
        mCxxLibrary = dom::CxxLibrary::obtain(mObject.mObject);
    }

    Object& mObject;
    dom::CxxLibrarySPtr mCxxLibrary;
};

struct CxxLibrarySet
{
    CxxLibrarySet(CxxLibraryRef& cxxLibraryRef)
        : mCxxLibraryRef(cxxLibraryRef)
    {
    }

    auto reset()
    {
        return [this](I& i1, I& i2) { mCxxLibraries.clear(); };
    }

    void operator()(I& i1, I& i2)
    {
        mCxxLibraries.insert(mCxxLibraryRef.mCxxLibrary);
    };

    CxxLibraryRef& mCxxLibraryRef;
    dom::CxxLibrarySet mCxxLibraries;
};

struct CxxLibrary
{
    CxxLibrary(Attribute& attribute, Directory& directory, FileSet& files)
        : mAttribute(attribute)
        , mDirectory(directory)
        , mFiles(files)
    {
    }

    auto resetHeader()
    {
        return [this](I& i1, I& i2) { mVisibility.reset(); };
    }

    auto attribute()
    {
        return [this](I& i1, I& i2) {
            if (mAttribute.mName == dom::CxxLibrary::gVisibility)
            {
                mVisibility = mAttribute.mAttribute;
            }
            else if (mAttribute.mName == dom::CxxLibrary::gDirectory)
            {
                mDirectory.mDirectory =
                    doim::FsDirectory::obtain(mDirectory.mDefaultDirectory,
                                              mAttribute.mValue->value());
            }
        };
    }

    static doim::CxxHeader::EVisibility visibility(const doim::AttributeSPtr& attribute)
    {
        if (attribute == nullptr || attribute->value() == dom::CxxLibrary::gPrivate)
            return doim::CxxHeader::EVisibility::kPrivate;
        if (attribute->value() == dom::CxxLibrary::gPublic)
            return doim::CxxHeader::EVisibility::kPublic;
        if (attribute->value() == dom::CxxLibrary::gProtected)
            return doim::CxxHeader::EVisibility::kProtected;
        ASSERT(false);
    }

    auto cxxHeaders()
    {
        return [this](I& i1, I& i2) {
            mCxxLibrary->updateCxxHeaders(visibility(mVisibility),
                                          mDirectory.mDirectory,
                                          mFiles.mFiles);
        };
    }
    auto protobufs()
    {
        return [this](I& i1, I& i2) {
            mCxxLibrary->updateProtobufsList(visibility(mVisibility),
                                             mDirectory.mDirectory,
                                             mFiles.mFiles);
        };
    }

    Attribute& mAttribute;
    doim::AttributeSPtr mVisibility;
    Directory& mDirectory;
    FileSet& mFiles;
    dom::CxxLibrarySPtr mCxxLibrary;
};

ECode DbsParser::parse(const doim::FsFileSPtr& dbsFile)
{
    // DLOG("Parse dbs file: {}", dbsFile->path());

    doim::FsDirectorySPtr location = dbsFile->directory();
    std::vector<string> errors;

    // White space
    Position position;
    auto r_ws = r_space | (r_endl >> e_ref(position));

    // Header end
    auto r_he = *r_ws & r_colon;
    // Structure end
    auto r_se = *r_ws & r_semicolon;

    // Attribute
    Attribute attribute;
    auto r_attributeName = *r_ws & r_ident() >> e_ref(attribute.name());
    auto r_attributeValue = *r_ws & (r_ident() | r_path) >> e_ref(attribute.value());
    auto r_attribute =
        (*r_ws & r_at & r_attributeName & *r_ws & r_equal & r_attributeValue) >>
        e_ref(attribute);

    // Directory
    Directory directory(location);
    auto r_setLocation = r_empty() >> e_ref(directory.reset());

    // File
    File file(directory);
    auto r_file = *r_ws & r_path >> e_ref(file);

    // Files
    FileSet files(file);
    auto r_files = (r_empty() >> e_ref(files.reset())) & *(r_file >> e_ref(files));

    // Object
    Object object(location);
    auto r_object = *r_ws & r_path >> e_ref(object);
    auto r_objectCxxLibrary = r_empty() >> e_ref(object.cxxLibrary()) & r_object;
    auto r_objectCxxProgram = r_empty() >> e_ref(object.cxxProgram()) & r_object;

    // Annex
    Annex annex(errors, file);
    auto r_annex =
        *r_ws & r_annexKeyword & r_he & r_setLocation & *(r_file >> e_ref(annex)) & r_se;

    // CxxLibrary Ref
    CxxLibraryRef cxxLibraryRef(object);
    auto r_cxxLibraryRef = r_objectCxxLibrary >> e_ref(cxxLibraryRef);

    // CxxLibraries
    CxxLibrarySet cxxLibraries(cxxLibraryRef);
    auto r_cxxLibraryItem = r_cxxLibraryRef >> e_ref(cxxLibraries);
    auto r_cxxLibrariesCat =
        *r_ws & r_cxxLibraryKeyword >> e_ref(cxxLibraries.reset()) & r_he;
    auto r_cxxLibraries = r_cxxLibrariesCat & *r_cxxLibraryItem & r_se;

    // CxxLibrary
    CxxLibrary cxxLibrary(attribute, directory, files);
    auto r_cxxHeaderAttribute = r_attribute >> e_ref(cxxLibrary.attribute());
    auto r_cxxHeaderCat =
        *r_ws & (r_cxxHeaderKeyword >> e_ref(cxxLibrary.resetHeader())) &
        (r_empty() >> e_ref(directory.reset())) & *r_cxxHeaderAttribute & r_he;
    auto r_cxxHeader =
        (r_cxxHeaderCat & r_files & r_se) >> e_ref(cxxLibrary.cxxHeaders());

    // CxxFiles
    auto r_cxxFiles = *r_ws & r_cxxFileKeyword & r_he & r_setLocation & r_files & r_se;

    // CxxLibrary CxxLibrary
    auto cxxLibraryX2Fn = [&cxxLibrary, &cxxLibraries](I& i1, I& i2) {
        cxxLibrary.mCxxLibrary->updateCxxLibraries(cxxLibraries.mCxxLibraries);
    };
    auto r_cxxLibraryX2 = r_cxxLibraries >> e_ref(cxxLibraryX2Fn);

    // CxxLibrary CxxFile
    auto cxxLibraryCxxFileFn = [&cxxLibrary, &files](I& i1, I& i2) {
        cxxLibrary.mCxxLibrary->updateCxxFilesList(files.mFiles);
    };
    auto r_cxxLibraryCxxFile = r_cxxFiles >> e_ref(cxxLibraryCxxFileFn);

    // CxxLibrary ProtobufFile

    auto r_cxxLibraryProtobufFileCat =
        *r_ws & (r_protobufFileKeyword >> e_ref(cxxLibrary.resetHeader())) &
        (r_empty() >> e_ref(directory.reset())) & *r_cxxHeaderAttribute & r_he;

    auto r_cxxLibraryProtobufFile =
        r_cxxLibraryProtobufFileCat & r_files >> e_ref(cxxLibrary.protobufs()) & r_se;
    ;

    // CxxLibrary CxxBinary
    auto cxxBinaryFn = [&file, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary.mCxxLibrary->updateBinary(file.mFile);
    };
    auto r_binary = *r_ws & r_binaryKeyword & r_he & r_setLocation &
                    (r_file >> e_ref(cxxBinaryFn)) & r_se;

    // ... CxxLibrary
    auto cxxLibraryNameFn = [&cxxLibraryRef, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary.mCxxLibrary = cxxLibraryRef.mCxxLibrary;
    };
    auto r_cxxLibraryName = r_cxxLibraryRef >> e_ref(cxxLibraryNameFn);

    auto cxxLibraryAttributeFn = [&attribute, &cxxLibrary](I& i1, I& i2) {
        cxxLibrary.mCxxLibrary->updateAttribute(attribute.mAttribute);
    };

    auto r_cxxLibraryAttribute = r_attribute >> e_ref(cxxLibraryAttributeFn);

    auto r_cxxLibraryCap =
        *r_ws & r_cxxLibraryKeyword & r_cxxLibraryName & *r_cxxLibraryAttribute & r_he;

    auto r_cxxLibrary = r_cxxLibraryCap &
                        *(r_cxxLibraryCxxFile | r_cxxLibraryProtobufFile | r_cxxHeader |
                          r_cxxLibraryX2 | r_binary) &
                        r_se;

    // CxxProgram
    dom::CxxProgramSPtr cxxProgram;

    // CxxProgram CxxLibrary
    auto cxxProgramCxxLibraryFn = [&cxxProgram, &cxxLibraries](I& i1, I& i2) {
        cxxProgram->updateCxxLibraries(cxxLibraries.mCxxLibraries);
    };
    auto r_cxxProgramCxxLibrary = r_cxxLibraries >> e_ref(cxxProgramCxxLibraryFn);

    // CxxProgram CxxFile
    auto cxxProgramCxxFileFn = [&cxxProgram, &files](I& i1, I& i2) {
        cxxProgram->updateCxxFilesList(files.mFiles);
    };
    auto r_cxxProgramCxxFile = r_cxxFiles >> e_ref(cxxProgramCxxFileFn);

    // ... CxxProgram
    auto cxxProgramNameFn = [&object, &cxxProgram](I& i1, I& i2) {
        cxxProgram = dom::CxxProgram::obtain(object.mObject);
        cxxProgram->updateName(object.mObject->name());
    };
    auto r_cxxProgramName = r_objectCxxProgram >> e_ref(cxxProgramNameFn);

    auto r_cxxProgramCap = *r_ws & r_cxxProgramKeyword & r_cxxProgramName & r_he;

    auto r_cxxProgram =
        r_cxxProgramCap & *(r_cxxProgramCxxFile | r_cxxProgramCxxLibrary) & r_se;

    const auto& r_dbs = ~r_annex & *(r_cxxLibrary | r_cxxProgram) & *r_ws & r_end();

    // Dbs file
    const auto& path = dbsFile->path();
    if (!boost::filesystem::exists(path))
        EHBan(kFailed, "File not found", path);

    std::ifstream fstream(dbsFile->path(nullptr).c_str());
    string content((std::istreambuf_iterator<char>(fstream)),
                   std::istreambuf_iterator<char>());

    r_dbs(content.begin(), content.end());

    // for (const auto& error : errors)
    //    std::cout << error << "\n";
    EHEnd;
}
}
