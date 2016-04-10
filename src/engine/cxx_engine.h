//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tool/cxx_compiler.h"
#include <memory>

namespace engine
{
class CxxEngine;

typedef std::shared_ptr<CxxEngine> CxxEngineSPtr;

class CxxEngine
{
public:
    CxxEngine(const tool::CxxCompilerSPtr& compiler);

private:
    tool::CxxCompilerSPtr mCompiler;
};
}
