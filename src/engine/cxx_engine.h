//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tool/cxx_clang_format.h"
#include "tool/cxx_compiler.h"
#include "tool/cxx_iwyu.h"
#include "tpool/task.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/db/db_key.hpp"
#include "doim/db/db_value.hpp"
#include "doim/fs/fs_directory.h"
#include <memory>

namespace engine
{
class CxxEngine;
typedef std::shared_ptr<CxxEngine> CxxEngineSPtr;

class CxxEngine : public std::enable_shared_from_this<CxxEngine>
{
public:
    CxxEngine(const tool::CxxClangFormatSPtr& formatter,
              const tool::CxxCompilerSPtr& compiler,
              const tool::CxxIwyuSPtr& iwyu);

    tpool::TaskSPtr updateDbTask(const tpool::TaskSPtr& task,
                                 const doim::DbKeySPtr& key,
                                 const doim::DbValueSPtr& value) const;

    tpool::TaskSPtr compileTask(const doim::FsDirectorySPtr& directory,
                                const doim::CxxObjectFileSPtr& objectFile);

    tpool::TaskSPtr buildObjects(const doim::FsDirectorySPtr& directory,
                                 const dom::CxxProgramSPtr& program);

    tpool::TaskSPtr build(const doim::FsDirectorySPtr& directory,
                          const dom::CxxProgramSPtr& program);

    tpool::TaskSPtr iwyuTask(const doim::FsDirectorySPtr& directory,
                             const doim::CxxFileSPtr& cxxFile);

    tpool::TaskSPtr iwyu(const doim::FsDirectorySPtr& directory,
                         const dom::CxxProgramSPtr& program);

private:
    tool::CxxClangFormatSPtr mFormatter;
    tool::CxxCompilerSPtr mCompiler;
    tool::CxxIwyuSPtr mIwyu;
};
}
