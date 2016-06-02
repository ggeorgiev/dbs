//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "parser/dbs/dbs_config_parser.h"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
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

ECode DbsConfigParser::parse(const doim::FsFileSPtr& dbsFile)
{
    EHEnd;
}
}
