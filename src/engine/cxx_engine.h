//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tool/cxx/cxx_clang_format.h"
#include "tool/cxx/cxx_compiler.h"
#include "tool/cxx/cxx_iwyu.h"
#include "tool/protobuf/protobuf_compiler.h"
#include "tpool/task.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_object_file.h"
#include "doim/cxx/cxx_program.h"
#include "doim/db/db_key.h"
#include "doim/fs/fs_directory.h"
#include "im/initialization_manager.hpp"
#include <map>
#include <memory>
#include <str>

namespace engine
{
class CxxEngine;
typedef shared_ptr<CxxEngine> CxxEngineSPtr;

class CxxEngine : public std::enable_shared_from_this<CxxEngine>
{
public:
    static int constexpr rank()
    {
        return doim::DbKey::rank() +
               doim::DbKey::levels() * im::InitializationManager::step();
    }

    CxxEngine(const tool::CxxClangFormatSPtr& formatter,
              const tool::CxxCompilerSPtr& compiler,
              const tool::CxxIwyuSPtr& iwyu,
              const tool::ProtobufCompilerSPtr& protobufCompiler);

    enum class EBuildFor
    {
        kDebug,
        kRelease,
        kProfile,
    };

    tpool::TaskSPtr build(EBuildFor buildFor,
                          const doim::FsDirectorySPtr& directory,
                          const dom::CxxProgramSPtr& program);

    tpool::TaskSPtr iwyu(const doim::FsDirectorySPtr& directory,
                         const dom::CxxProgramSPtr& program);

    string buildScript(EBuildFor buildFor,
                       const doim::FsDirectorySPtr& directory,
                       const dom::CxxProgramSPtr& program);

private:
    static doim::DbKeySPtr gBuildDbKey;
    static doim::DbKeySPtr gDebugDbKey;
    static doim::DbKeySPtr gReleaseDbKey;
    static doim::DbKeySPtr gProfileDbKey;

    static std::map<EBuildFor, string> gSubDirectory;
    static std::map<EBuildFor, doim::CxxProgram::EPurpose> gProgramPurpose;

    static bool initDbKeyPurpose();
    static std::map<EBuildFor, doim::DbKeySPtr> gDbKeyPurpose;

    template <typename T>
    tpool::TaskSPtr updateDbTask(const tpool::TaskSPtr& task,
                                 const doim::DbKeySPtr& key,
                                 const T& crcTask) const;

    tpool::TaskSPtr compileOrigin(const doim::DbKeySPtr& ancestor,
                                  const doim::CxxSourceOriginSPtr& origin,
                                  const doim::FsDirectorySPtr& directory);

    tpool::TaskSPtr compileTask(const doim::DbKeySPtr& ancestor,
                                const doim::FsDirectorySPtr& directory,
                                const doim::CxxObjectFileSPtr& objectFile);

    tpool::TaskSPtr compileObjects(const doim::DbKeySPtr& ancestor,
                                   const doim::FsDirectorySPtr& directory,
                                   const doim::CxxProgramSPtr& program);

    tpool::TaskSPtr iwyuTask(const doim::FsDirectorySPtr& directory,
                             const doim::CxxFileSPtr& cxxFile);

    tool::CxxClangFormatSPtr mFormatter;
    tool::CxxCompilerSPtr mCompiler;
    tool::CxxIwyuSPtr mIwyu;

    tool::ProtobufCompilerSPtr mProtobufCompiler;
};
}
