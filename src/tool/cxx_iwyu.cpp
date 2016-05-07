//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tool/cxx_iwyu.h"
#include "tool/cxx_compiler.h"
#include "task/manager.h"
#include "task/sys/parse_stdout_task.h"
#include "tpool/task_callback.h"
#include "dom/cxx/cxx_library.h"
#include "dom/cxx/cxx_program.h"
#include "doim/cxx/cxx_file.h"
#include "doim/fs/fs_file.h"
#include "doim/manager.h"
#include "doim/sys/argument.hpp"
#include "doim/sys/command.h"
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>

namespace tool
{
CxxIwyu::CxxIwyu(const doim::SysExecutableSPtr& tool)
    : mTool(tool)
{
}

tpool::TaskSPtr CxxIwyu::iwyuCommand(const doim::FsDirectorySPtr& directory,
                                     const doim::CxxFileSPtr& cxxFile) const
{
    auto arguments =
        CxxCompiler::includeArguments(directory, cxxFile->cxxIncludeDirectories());

    auto argument_cxxflags = doim::gManager->obtainArgument(
        "-std=c++11 -stdlib=libc++ -O0 -g "
        "-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/"
        "MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk");
    arguments->insert(argument_cxxflags);

    const std::string& file = cxxFile->file()->path(directory);

    auto argument_file = doim::gManager->obtainArgument(file);
    arguments->insert(argument_file);
    arguments = doim::gManager->unique(arguments);

    auto command = std::make_shared<doim::SysCommand>(mTool, arguments);
    command = doim::gManager->unique(command);

    auto fn = [](int exit, const std::string& stdout) -> ECode {
        if (exit == 0)
        {
            ELOG("Unexpected exit code from iwyu tool: {}", exit);
            EHBan(kUnexpected);
        }

        if (stdout.find("has correct #includes/fwd-decls") != std::string::npos)
            EHEnd;

        static std::regex removeItemsRegex(
            "(?:^|\n)(.*?) should remove these lines:"
            "(?:[\\s\\r\\n]+- #include.*?\\/\\/ lines \\d+-\\d+)+");
        static std::regex removeItemRegex("- #include.*?\\/\\/ lines (\\d+)-\\d+");

        auto its = std::sregex_iterator(stdout.begin(), stdout.end(), removeItemsRegex);
        for (std::sregex_iterator items = its; items != std::sregex_iterator(); ++items)
        {
            std::smatch smatchs = *items;
            const auto& removes = smatchs.str();
            auto it =
                std::sregex_iterator(removes.begin(), removes.end(), removeItemRegex);

            for (std::sregex_iterator item = it; item != std::sregex_iterator(); ++item)
            {
                std::smatch smatch = *item;
                ELOG("\n{}:{}:1: error: {} ",
                     smatchs[1].str(),
                     smatch[1].str(),
                     smatch[0].str());
            }
        }

        static std::regex addItemsRegex("(?:^|\n)(.*?) should add these lines:"
                                        "(?:[\\s\\r\\n]+#include.*?\\/\\/[^\\n]*)+");
        static std::regex addItemRegex("(#include[^\\n]+)");

        its = std::sregex_iterator(stdout.begin(), stdout.end(), addItemsRegex);
        for (std::sregex_iterator items = its; items != std::sregex_iterator(); ++items)
        {
            std::smatch smatchs = *items;
            const auto& adds = smatchs.str();
            auto it = std::sregex_iterator(adds.begin(), adds.end(), addItemRegex);

            for (std::sregex_iterator item = it; item != std::sregex_iterator(); ++item)
            {
                std::smatch smatch = *item;
                ELOG("\n{}:1:1: error: {} ", smatchs[1].str(), smatch[1].str());
            }
        }

        ELOG("\n{}", stdout);
        EHBan(kFailed);
    };

    return task::gManager->valid(
        std::make_shared<task::ParseStdoutTask>(command,
                                                nullptr,
                                                rtti::RttiInfo<CxxIwyu>::classId(),
                                                fn,
                                                "Iwyu " + file));
}
}
