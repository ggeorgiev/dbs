//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "task/element_manager.hpp"
#include "tpool/task.h"
#include "logex/log.h"
#include "option/verbose.h"
#include "doim/tag/tag.h"
#include "rtti/class_rtti.hpp"
#include <boost/functional/hash.hpp>
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
    typedef std::tuple<Args...> Tuple;

    static shared_ptr<T> valid(const Tuple& args)
    {
        return gElementManager->valid(std::make_shared<T>(args));
    }

    static shared_ptr<T> valid(const Args&... args)
    {
        return gElementManager->valid(enable_make_shared<T>::make(args...));
    }

    Element(const Args&... args)
        : tpool::Task(0)
        , mArgs(args...)
    {
        ASSERT(static_cast<T*>(this)->check());
    }

    Element(const Tuple& args)
        : tpool::Task(0)
        , mArgs(args)
    {
    }

    virtual string description() const = 0;

    virtual doim::TagSetSPtr tags() const = 0;

    shared_ptr<T> rerun()
    {
        shared_ptr<T> key(shared_ptr<T>(), static_cast<T*>(this));
        gElementManager->erase(key);

        const auto task = T::valid(mArgs);

        return task;
    }

    void onStart() const override
    {
        LOGEX(tags()->combine(doim::gRunTagSet), "[ RUN      ] {}", description());
    }

    void onFinish(ECode code) const override
    {
        LOGEX(tags()->combine(doim::gDoneTagSet),
              "{} {}",
              (code == err::kSuccess) ? "[       OK ]" : "[   FAILED ]",
              description());
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
            boost::hana::for_each(object->mArgs, hashItem);
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
