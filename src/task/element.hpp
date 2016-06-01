//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element_manager.hpp"
#include "tpool/task.h"
#include "option/verbose.h"
#include "doim/tag/tag.h"
#include "log/log.h"
#include "rtti/class_rtti.hpp"
#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm.hpp>
#include <functional>
#include <memory>
#include <shared_ptr>
#include <string_view>
#include <tuple>
#include <unordered>
#include <vector>

namespace task
{
template <typename T, typename... Args>
class Element : public enable_make_shared<T>, public tpool::Task, public rtti::RttiInfo<T>
{
public:
    static shared_ptr<T> valid(const Args&... args)
    {
        return gElementManager->valid(enable_make_shared<T>::make(args...));
    }

    typedef std::tuple<Args...> Tuple;

    Element(const Args&... args)
        : tpool::Task(0)
        , mArgs(args...)
    {
    }

    virtual string description() const = 0;

    virtual doim::TagSet tags() const
    {
        return doim::TagSet{doim::gTaskTag};
    }

    void onStart() const override
    {
        doim::TagSetSPtr runTags = doim::TagSet::make(tags());
        runTags->insert(doim::gRunTag);
        runTags = doim::TagSet::unique(runTags);

        if (opt::gVerbose->isVisible(runTags))
            ILOG("[ RUN      ] {}", description());
    }

    void onFinish(ECode code) const override
    {
        doim::TagSetSPtr doneTags = doim::TagSet::make(tags());
        doneTags->insert(doim::gDoneTag);
        doneTags = doim::TagSet::unique(doneTags);

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
                boost::hash<Item> hash;
                boost::hash_combine(mSeed, hash(item));
            }

            mutable std::size_t mSeed;
        };

        std::size_t operator()(const shared_ptr<T>& object) const
        {
            HashItem hashItem(mClassIdHasher(T::classId()));
            boost::fusion::for_each(object->mArgs, hashItem);
            return hashItem.mSeed;
        }

        bool operator()(const shared_ptr<T>& object1, const shared_ptr<T>& object2) const
        {
            return object1->mArgs == object2->mArgs;
        }

    private:
        std::hash<rtti::ClassId> mClassIdHasher;
    };

protected:
    Tuple mArgs;

    static shared_ptr<ElementManager<T, Args...>> gElementManager;
};

template <typename T, typename... Args>
shared_ptr<ElementManager<T, Args...>> Element<T, Args...>::gElementManager =
    im::InitializationManager::subscribe(gElementManager);
}
