//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "tpool/task.h"
#include "option/verbose.h"
#include "doim/tag/tag.h"
#include "log/log.h" // IWYU pragma: keep
#include "rtti/class_rtti.hpp"
#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>
#include <experimental/string_view>
#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace task
{
template <typename T, typename... Args>
class Base : public tpool::Task, public rtti::RttiInfo<T>
{
public:
    typedef std::tuple<Args...> Tuple;

    Base(const Args&... args)
        : tpool::Task(0)
        , mArgs(args...)
    {
    }

    virtual std::string description() const = 0;

    virtual doim::TagSet tags() const
    {
        return doim::TagSet{doim::gTaskTag};
    }

    void onStart() const override
    {
        doim::TagSet runTags = tags();
        runTags.insert(doim::gRunTag);
        if (opt::gVerbose->isVisible(runTags))
            ILOG("[ RUN      ] {}", description());
    }

    void onFinish(ECode code) const override
    {
        doim::TagSet doneTags = tags();
        doneTags.insert(doim::gDoneTag);
        if (opt::gVerbose->isVisible(doneTags))
        {
            ILOG("{} {}",
                 (code == err::kSuccess) ? "[       OK ]" : "[   FAILED ]",
                 description());
        }
    }

    struct Hasher
    {
        struct HashItem
        {
            HashItem(std::size_t seed)
                : mSeed(seed)
            {
            }

            template <typename Item>
            void operator()(const Item& item) const
            {
                std::hash<Item> hash;
                boost::hash_combine(mSeed, hash(item));
            }

            mutable std::size_t mSeed;
        };

        std::size_t operator()(const std::shared_ptr<T>& object) const
        {
            HashItem hashItem(mClassIdHasher(T::classId()));
            boost::fusion::for_each(object->mArgs, hashItem);
            return hashItem.mSeed;
        }

        bool operator()(const std::shared_ptr<T>& object1,
                        const std::shared_ptr<T>& object2) const
        {
            return object1->mArgs == object2->mArgs;
        }

    private:
        std::hash<rtti::ClassId> mClassIdHasher;
    };

protected:
    Tuple mArgs;
};
}
