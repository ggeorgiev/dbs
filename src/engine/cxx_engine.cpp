//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "tool/cxx_compiler.h"

namespace engine
{
CxxEngine::CxxEngine(const tool::CxxCompilerSPtr& compiler)
    : mCompiler(compiler)
{
}
}
