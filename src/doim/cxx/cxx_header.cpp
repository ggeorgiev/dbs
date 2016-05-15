//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/cxx/cxx_header.h"
#include "err/err_assert.h"
#include <ostream>

namespace doim
{
CxxHeader::CxxHeader(const EType type,
                     const FsFileSPtr& file,
                     const CxxIncludeDirectorySetSPtr& cxxIncludeDirectories)
    : Element(type, file, cxxIncludeDirectories)
{
    ASSERT(file->isUnique());
    ASSERT(cxxIncludeDirectories->isUnique());
}

std::ostream& operator<<(std::ostream& out, const CxxHeader& header)
{
    out << "header { type: ";
    switch (header.type())
    {
        case CxxHeader::EType::kUser:
            out << "user";
            break;
        case CxxHeader::EType::kSystem:
            out << "system";
            break;
    }
    out << ", file: " << header.file()->path() << "}" << std::endl;
    return out;
}
}
