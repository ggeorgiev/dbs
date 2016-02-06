//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/sys/argument.hpp"
#include "doim/sys/executable.hpp"

namespace doim
{
class SysCommand;
typedef std::shared_ptr<SysCommand> SysCommandSPtr;

class SysCommand
{
public:
    SysCommand(const SysExecutableSPtr& executable, const SysArgumentSetSPtr& arguments)
        : mExecutable(executable)
        , mArguments(arguments)
    {
    }

    const SysExecutableSPtr& executable() const
    {
        return mExecutable;
    }

    const SysArgumentSetSPtr& arguments() const
    {
        return mArguments;
    }

    const std::string& string() const
    {
        if (mString.empty())
        {
            mString = executable()->path();

            for (const auto& argument : *arguments())
                mString += " " + argument->value();
        }

        return mString;
    }

    struct Hasher
    {
        std::size_t operator()(const SysCommandSPtr& command) const
        {
            auto seed = hashE(command->executable());
            boost::hash_combine(seed, hashAS(command->arguments()));
            return seed;
        }

        bool operator()(const SysCommandSPtr& command1,
                        const SysCommandSPtr& command2) const
        {
            return command1->executable() == command2->executable() &&
                   command1->arguments() == command2->arguments();
        }

        std::hash<SysExecutableSPtr> hashE;
        std::hash<SysArgumentSetSPtr> hashAS;
    };

private:
    SysExecutableSPtr mExecutable;
    SysArgumentSetSPtr mArguments;

    mutable std::string mString;
};

typedef std::unordered_set<SysCommandSPtr> SysCommandSet;
typedef std::shared_ptr<SysCommandSet> SysCommandSetSPtr;

struct SysCommandSetHasher
{
    std::size_t operator()(const SysCommandSetSPtr& commands) const
    {
        std::vector<SysCommandSPtr> vector(commands->size());
        vector.insert(vector.begin(), commands->begin(), commands->end());
        std::sort(vector.begin(), vector.end());

        std::size_t seed = 0;
        for (const auto& command : vector)
            boost::hash_combine(seed, hashSC(command));

        return seed;
    }

    bool operator()(const SysCommandSetSPtr& commands1,
                    const SysCommandSetSPtr& commands2) const
    {
        return *commands1 == *commands2;
    }

    std::hash<SysCommandSPtr> hashSC;
};
}
