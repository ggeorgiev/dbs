//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "engine/cxx_engine.h"
#include "task/cxx/cxx_program_crc_task.h"
#include "task/db/db_put_task.h"
#include "task/manager.h"
#include "tpool/task_callback.h"
#include "tool/cxx_compiler.h"
#include "db/database.h"

namespace engine
{
CxxEngine::CxxEngine(const tool::CxxCompilerSPtr& compiler)
    : mCompiler(compiler)
{
}

tpool::TaskSPtr CxxEngine::build(const doim::FsDirectorySPtr& directory,
                                 const dom::CxxProgramSPtr& program)
{
    const auto& intermediate = doim::gManager->obtainDirectory(directory, "build");
    const auto& cxxProgram = program->cxxProgram(directory, intermediate);

    auto crcTask =
        task::gManager->valid(std::make_shared<task::CxxProgramCrcTask>(cxxProgram));
    task::gTPool->ensureScheduled(crcTask);

    auto self = shared_from_this();
    tpool::TaskCallback::Function onFinish = [this, self, directory, program](
        const tpool::TaskSPtr& task) -> ECode {
        auto key = std::make_shared<doim::DbKey>("cxx_program:" + program->name());
        key = doim::gManager->unique(key);

        math::Crcsum crc;
        db::gDatabase->get(key->bytes(), crc);

        auto crcTask = std::static_pointer_cast<task::CxxProgramCrcTask>(task);
        if (crcTask->crc() == crc)
        {
            DLOG("Cxx program '{}' is already built.", program->name());
            EHEnd;
        }
        EHTest(mCompiler->commands(directory, program));

        auto value = std::make_shared<doim::DbValue>(crcTask->crc());
        auto updateTask =
            task::gManager->valid(std::make_shared<task::DbPutTask>(key, value));
        task::gTPool->ensureScheduled(updateTask);
        EHTest(updateTask->join());
        EHEnd;
    };

    auto task = std::make_shared<tpool::TaskCallback>(0, crcTask, onFinish);
    task::gTPool->ensureScheduled(crcTask);
    return task;
}
}
