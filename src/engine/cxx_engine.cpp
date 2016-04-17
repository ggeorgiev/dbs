//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "tool/cxx_compiler.h"
#include "db/database.h"

namespace engine
{
CxxEngine::CxxEngine(const tool::CxxCompilerSPtr& compiler)
    : mCompiler(compiler)
{
}

ECode CxxEngine::build(const doim::FsDirectorySPtr& directory,
                       const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    auto task =
        task::gManager->valid(std::make_shared<task::CxxProgramCrcTask>(cxxProgram));
    task::gTPool->ensureScheduled(task);
    EHTest(task->join(), program->name());

    auto key = std::make_shared<doim::DbKey>("cxx_program:" + program->name());
    key = doim::gManager->unique(key);

    math::Crcsum crc;
    db::gDatabase->get(key->bytes(), crc);

    if (task->crc() == crc)
    {
        DLOG("Cxx program '{}' is already built.", program->name());
        EHEnd;
    }

    EHTest(mCompiler->commands(directory, program));

    auto value = std::make_shared<doim::DbValue>(task->crc());
    auto updateTask =
        task::gManager->valid(std::make_shared<task::DbPutTask>(key, value));
    task::gTPool->ensureScheduled(updateTask);
    EHTest(updateTask->join());
    EHEnd;
}
}
