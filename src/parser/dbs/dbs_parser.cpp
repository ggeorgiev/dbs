//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_attribute.hpp"
#include "parser/dbs/e_cxx_library.hpp"
#include "parser/dbs/e_cxx_program.hpp"
#include "parser/dbs/e_depository.hpp"
#include "parser/dbs/e_directory.hpp"
#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_file_set.hpp"
#include "parser/dbs/e_object.hpp"
#include "parser/dbs/e_particle.hpp"
#include "parser/dbs/e_position.hpp"
#include "parser/dbs/e_url.hpp"
#include "doim/fs/fs_directory.h"
#include <boost/filesystem/operations.hpp>
#include <fstream> // IWYU pragma: keep
#include <iterator>
#include <str>
#include <vector>
#include <axe.h> // IWYU pragma: keep

namespace parser
{
static auto r_protobufPluginKw = r_str("protobuf_plugin");

ECode DbsParser::parse(const doim::FsFileSPtr& dbsFile)
{
    // DLOG("Parse dbs file: {}", dbsFile->path());

    doim::FsDirectorySPtr location = dbsFile->directory();
    std::vector<string> errors;

    // White space
    Position position;
    const auto& r_ws = position.r_ws();
    const auto& r_he = Particle::r_he(r_ws);
    const auto& r_se = Particle::r_se(r_ws);

    // Attribute
    Attribute attribute;
    auto r_attributeName = r_ws & r_ident() >> attribute.name();
    auto r_attributeValue = r_ws & (r_ident() | r_path) >> attribute.value();
    auto r_attribute =
        (r_ws & r_at & r_attributeName & r_ws & r_equal & r_attributeValue) >>
        e_ref(attribute);

    // Directory
    Directory directory(location);
    auto r_resetDir = r_empty() >> directory.reset();

    // File
    File file(directory);
    auto r_file = r_ws & r_path >> e_ref(file);

    // Files
    FileSet files(file);
    auto r_files = r_empty() >> files.reset() & *(r_file >> e_ref(files));

    // Url
    Url url;
    auto r_url = r_ws & (r_ident() & r_colon & r_slash & r_slash & r_path) >> e_ref(url);

    // Object
    Object object(location);
    auto r_object = r_ws & r_path >> e_ref(object);
    auto r_objectCxxLibrary = r_empty() >> object.cxxLibrary() & r_object;
    auto r_objectCxxProgram = r_empty() >> object.cxxProgram() & r_object;
    auto r_objectDepository = r_empty() >> object.depository() & r_object;

    // Annex
    Annex annex(errors, file);
    auto r_annex =
        r_ws & r_annexKw & r_he & r_resetDir & *(r_file >> e_ref(annex)) & r_se;

    // Depository Ref
    DepositoryRef depositoryRef(object);
    auto r_depositoryRef = r_objectDepository >> e_ref(depositoryRef);

    // Depository
    Depository depository(url, depositoryRef);
    auto r_depositoryName = r_depositoryRef >> depository.name();
    auto r_giturl = r_ws & r_urlKw & r_he & r_url >> depository.gitUrl() & r_se;
    auto r_gittag =
        r_ws & r_tagKw & r_he & r_ws & r_gittagValue >> depository.gitTag() & r_se;
    auto r_git = r_ws & r_gitKw & r_he & r_giturl & r_gittag & r_se;
    auto r_depository = r_ws & r_depositoryKw & r_depositoryName & r_he & r_git & r_se;

    // CxxLibrary Ref
    CxxLibraryRef cxxLibraryRef(object);
    auto r_cxxLibraryRef = r_objectCxxLibrary >> e_ref(cxxLibraryRef);

    // CxxLibraries
    CxxLibrarySet cxxLibraries(cxxLibraryRef);
    auto r_cxxLibraryItem = r_cxxLibraryRef >> e_ref(cxxLibraries);
    auto r_cxxLibraries =
        r_ws & r_cxxLibraryKw >> cxxLibraries.reset() & r_he & *r_cxxLibraryItem & r_se;

    // CxxLibrary
    CxxLibrary cxxLibrary(attribute, directory, file, files, cxxLibraries, cxxLibraryRef);
    auto r_cxxHeaderAttribute = r_attribute >> cxxLibrary.attribute();

    // CxxHeaders
    auto r_cxxHeader = (r_ws & r_cxxHeaderKw >> cxxLibrary.resetHeader() &
                        *r_cxxHeaderAttribute & r_he & r_files & r_se) >>
                       cxxLibrary.cxxHeaders();

    // CxxFiles
    auto r_cxxFiles = r_ws & r_cxxFileKw & r_he & r_resetDir & r_files & r_se;
    auto r_cxxLibraryCxxFile = r_cxxFiles >> cxxLibrary.files();

    // CxxLibrary CxxLibrary
    auto r_cxxLibraryX2 = r_cxxLibraries >> cxxLibrary.libraries();

    // CxxLibrary ProtobufFile
    auto r_cxxLibraryProtobufFile = r_ws & r_protobufFileKw >> cxxLibrary.resetHeader() &
                                    *r_cxxHeaderAttribute & r_he &
                                    r_files >> cxxLibrary.protobufs() & r_se;

    // CxxLibrary CxxBinary
    auto r_binary =
        r_ws & r_binaryKw & r_he & r_resetDir & r_file >> cxxLibrary.binary() & r_se;

    // ... CxxLibrary
    auto r_cxxLibraryName = r_cxxLibraryRef >> cxxLibrary.name();
    auto r_cxxLibraryAttribute = r_attribute >> cxxLibrary.updateAttribute();
    auto r_cxxLibrary = r_ws & r_cxxLibraryKw & r_cxxLibraryName &
                        *r_cxxLibraryAttribute & r_he &
                        *(r_cxxLibraryCxxFile | r_cxxLibraryProtobufFile | r_cxxHeader |
                          r_cxxLibraryX2 | r_binary) &
                        r_se;

    // CxxProgram
    CxxProgram cxxProgram(object, files, cxxLibraries);

    // CxxProgram CxxLibrary
    auto r_cxxProgramCxxLibrary = r_cxxLibraries >> cxxProgram.libraries();

    // CxxProgram CxxFile
    auto r_cxxProgramCxxFile = r_cxxFiles >> cxxProgram.files();

    // ... CxxProgram
    auto r_cxxProgramName = r_objectCxxProgram >> cxxProgram.name();
    auto r_cxxProgram = r_ws & r_cxxProgramKw & r_cxxProgramName & r_he &
                        *(r_cxxProgramCxxFile | r_cxxProgramCxxLibrary) & r_se;

    const auto& r_dbs =
        ~r_annex & *(r_depository | r_cxxLibrary | r_cxxProgram) & r_ws & r_end();

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
