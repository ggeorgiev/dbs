//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "tool/cxx/cxx_iwyu.h"
#include "tool/cxx/cxx_compiler.h"
#include "task/sys/parse_stdout_task.h"
#include "doim/cxx/cxx_file.h"
#include "doim/cxx/cxx_header.h"
#include "doim/cxx/cxx_include_directory.h"
#include "doim/fs/fs_file.h"
#include "doim/sys/sys_argument.h"
#include "doim/sys/sys_command.h"
#include "doim/set.hpp"
#include "err/err.h"
#include "log/log.h"
#include "rtti/class_rtti.hpp"
#include <functional>
#include <memory>
#include <regex>
#include <str>
#include <string_view>
#include <vector>

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
    arguments->insert(CxxCompiler::gStdCpp14Argument);
    arguments->insert(CxxCompiler::gOptimizationLevel0Argument);

    const string& file = cxxFile->file()->path(directory);

    auto argument_file = doim::SysArgument::unique(file);
    arguments->insert(argument_file);
    arguments = doim::SysArgumentSet::unique(arguments);

    auto command = doim::SysCommand::unique(nullptr, mTool, arguments);

    auto fn = [directory, cxxFile](int exit, const string& output) -> ECode {
        if (exit == 0)
        {
            ELOG("Unexpected exit code from iwyu tool: {}", exit);
            EHBan(kUnexpected);
        }

        bool success = true;
        static std::regex removeItemsRegex(
            "(?:^|\n)(.*?) should remove these lines:"
            "(?:[\\s\\r\\n]+- #include.*?\\/\\/ lines \\d+-\\d+)+");
        static std::regex removeItemRegex("- (#include.*?)\\s*\\/\\/ lines (\\d+)-\\d+");

        auto its = std::sregex_iterator(output.begin(), output.end(), removeItemsRegex);
        for (std::sregex_iterator items = its; items != std::sregex_iterator(); ++items)
        {
            std::smatch smatchs = *items;
            const auto& removes = smatchs.str();
            auto it =
                std::sregex_iterator(removes.begin(), removes.end(), removeItemRegex);

            for (std::sregex_iterator item = it; item != std::sregex_iterator(); ++item)
            {
                success = false;
                std::smatch smatch = *item;
                ELOG("\n{}:{}:1: error: extra {} ",
                     smatchs[1].str(),
                     smatch[2].str(),
                     smatch[1].str());
            }
        }

        static std::regex addItemsRegex("(?:^|\n)(.*?) should add these lines:"
                                        "(?:[\\s\\r\\n]+#include.*?\\/\\/[^\\n]*)+");
        static std::regex addItemRegex("(#include\\s[\"<]([^\">]+)[^\\n]+)");

        struct Warning
        {
            std::string erroredFile;
            std::string missingHeader;
        };

        std::vector<Warning> warnings;

        its = std::sregex_iterator(output.begin(), output.end(), addItemsRegex);
        for (std::sregex_iterator items = its; items != std::sregex_iterator(); ++items)
        {
            std::smatch smatchs = *items;
            const auto& adds = smatchs.str();
            auto it = std::sregex_iterator(adds.begin(), adds.end(), addItemRegex);

            for (std::sregex_iterator item = it; item != std::sregex_iterator(); ++item)
            {
                std::smatch smatch = *item;
                warnings.push_back(Warning{smatchs[1].str(), smatch[2].str()});
            }
        }

        for (const auto& warning : warnings)
        {
            auto file = doim::FsFile::find(directory, warning.erroredFile);
            if (file == nullptr)
            {
                success = false;
                ELOG("IWYU reports issues for unknown file {}", warning.erroredFile);
                continue;
            }
            doim::CxxIncludeDirectory::CxxHeaderInfo headerInfo;
            if (cxxFile->file() == file)
            {
                EHLog(doim::CxxIncludeDirectory::findHeaderDeep(
                    string_view(warning.missingHeader),
                    nullptr,
                    cxxFile->cxxIncludeDirectories(),
                    headerInfo));
            }
            else
            {
                EHLog(doim::CxxIncludeDirectory::findHeaderDeep(
                    file, nullptr, cxxFile->cxxIncludeDirectories(), headerInfo));

                if (headerInfo.mHeader == nullptr)
                {
                    success = false;
                    ELOG("IWYU reports issues for unknown file {}", warning.erroredFile);
                    continue;
                }

                EHLog(doim::CxxIncludeDirectory::findHeaderDeep(
                    string_view(warning.missingHeader),
                    headerInfo.mIncludeDirectory,
                    headerInfo.mHeader->cxxIncludeDirectories(),
                    headerInfo));
            }

            if (headerInfo.mHeader == nullptr)
            {
                success = false;
                ELOG("IWYU suggest adding unknown header {}", warning.missingHeader);
                continue;
            }

            if (headerInfo.mHeader->visibility() ==
                doim::CxxHeader::EVisibility::kProtected)
            {
                TLOG("IWYU suggest protected header {}, ignore those",
                     warning.missingHeader);
                continue;
            }

            success = false;
            ELOG("\n{}:1:1: warning: missing {} ",
                 warning.erroredFile,
                 warning.missingHeader);
        }

        if (!success)
        {
            ELOG("\n{}", output);
            EHBan(kFailed);
        }

        EHEnd;
    };

    return task::ParseStdoutTask::valid(
        command, nullptr, rtti::RttiInfo<CxxIwyu>::classId(), fn, "Iwyu " + file);
}
}
