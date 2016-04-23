//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tool/cxx_compiler.h"
#include <memory>

namespace engine
{
class CxxEngine;

typedef std::shared_ptr<CxxEngine> CxxEngineSPtr;

class CxxEngine : public std::enable_shared_from_this<CxxEngine>
{
public:
    CxxEngine(const tool::CxxCompilerSPtr& compiler);

    tpool::TaskSPtr build(const doim::FsDirectorySPtr& directory,
                          const dom::CxxProgramSPtr& program);

private:
    tool::CxxCompilerSPtr mCompiler;
};
}
