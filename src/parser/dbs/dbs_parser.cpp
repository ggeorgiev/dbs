//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_parser.h"
#include "parser/dbs/e_annex.hpp"
#include "parser/dbs/e_cxx_library.hpp"
#include "parser/dbs/e_cxx_program.hpp"
#include "parser/dbs/e_depository.hpp"
#include "parser/dbs/e_position.hpp"
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

    Position position;
    const auto& r_ws = position.r_ws();

    Annex annex(location, errors);
    Depository depository(location);
    CxxLibrary cxxLibrary(location);
    CxxProgram cxxProgram(location);

    const auto& r_dbs =
        ~annex.rule(r_ws) &
        *(depository.rule(r_ws) | cxxLibrary.rule(r_ws) | cxxProgram.rule(r_ws)) & r_ws &
        r_end();

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
