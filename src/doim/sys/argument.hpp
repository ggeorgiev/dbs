//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include <boost/functional/hash.hpp>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace doim
{
class SysArgument;
typedef std::shared_ptr<SysArgument> SysArgumentSPtr;

class SysArgument
{
public:
    SysArgument(const std::string& value)
        : mValue(value)
    {
    }

    const std::string& value() const
    {
        return mValue;
    }

    struct Hasher
    {
        std::size_t operator()(const SysArgumentSPtr& argument) const
        {
            return hashString(argument->value());
        }

        bool operator()(const SysArgumentSPtr& argument1,
                        const SysArgumentSPtr& argument2) const
        {
            return argument1->value() == argument2->value();
        }

        std::hash<std::string> hashString;
    };

private:
    std::string mValue;
};

typedef std::unordered_set<SysArgumentSPtr> SysArgumentSet;
typedef std::shared_ptr<SysArgumentSet> SysArgumentSetSPtr;

struct SysArgumentSetHasher
{
    std::size_t operator()(const SysArgumentSetSPtr& arguments) const
    {
        std::vector<SysArgumentSPtr> vector(arguments->size());
        vector.insert(vector.begin(), arguments->begin(), arguments->end());
        std::sort(vector.begin(), vector.end());

        std::size_t seed = 0;
        for (const auto& file : vector)
            boost::hash_combine(seed, hashA(file));

        return seed;
    }

    bool operator()(const SysArgumentSetSPtr& arguments1,
                    const SysArgumentSetSPtr& arguments2) const
    {
        return *arguments1 == *arguments2;
    }

    std::hash<SysArgumentSPtr> hashA;
};
}
