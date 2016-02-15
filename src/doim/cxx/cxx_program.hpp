//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/cxx/cxx_object_file.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class CxxProgram;
typedef std::shared_ptr<CxxProgram> CxxProgramSPtr;

class CxxProgram
{
public:
    CxxProgram(const FsFileSPtr& file, const CxxObjectFileSetSPtr& cxxObjectFiles)
        : mFile(file)
        , mCxxObjectFiles(cxxObjectFiles)
    {
    }

    const FsFileSPtr& file() const
    {
        return mFile;
    }

    const CxxObjectFileSetSPtr& cxxObjectFiles()
    {
        return mCxxObjectFiles;
    }

    struct Hasher
    {
        std::size_t operator()(const CxxProgramSPtr& program) const
        {
            auto seed = hashFF(program->file());
            boost::hash_combine(seed, hashCFOS(program->cxxObjectFiles()));
            return seed;
        }

        bool operator()(const CxxProgramSPtr& program1,
                        const CxxProgramSPtr& program2) const
        {
            return program1->cxxObjectFiles() == program2->cxxObjectFiles();
        }

        std::hash<FsFileSPtr> hashFF;
        std::hash<CxxObjectFileSetSPtr> hashCFOS;
    };

private:
    CxxObjectFileSetSPtr mCxxObjectFiles;
    FsFileSPtr mFile;
};
}
