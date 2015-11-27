//  Copyright © 2015 George Georgiev. All rights reserved.
//

#pragma once

#include <map>
#include <vector>

namespace im
{
class InitializationManager
{
public:
    typedef std::function<bool()> InitializeFunction;
    typedef std::function<void()> UninitializeFunction;

    struct InitializeRecord
    {
        InitializeFunction mFnInitialize;
        UninitializeFunction mFnUninitialize;
    };

    typedef std::multimap<int, InitializeRecord> Initializers;

    InitializationManager() { initialize_objects(); }
    ~InitializationManager() { uninitialize_objects(); }
    static inline int rank_base() { return 1; }
    static inline int rank_step() { return 2; }
    template <typename T>
    static inline T subscribe(int rank,
                              InitializeFunction fnInitializer,
                              UninitializeFunction fnUninitializer)
    {
        add(rank, fnInitializer, fnUninitializer);
        return T();
    }

    template <typename T>
    static inline std::shared_ptr<T> subscribe(std::shared_ptr<T>& shared)
    {
        add(T::initialization_rank(),
            [&shared]() -> bool {
                shared = std::make_shared<T>();
                return true;
            },
            [&shared]() { shared.reset(); });
        return nullptr;
    }

private:
    static void add(int rank,
                    InitializeFunction fnInitializer,
                    UninitializeFunction fnUninitializer)
    {
        Initializers::value_type value(rank, InitializeRecord());
        value.second.mFnInitialize = fnInitializer;
        value.second.mFnUninitialize = fnUninitializer;

        _initializers().insert(value);
    }

    void initialize_objects()
    {
        // initialize all global objects
        auto initializers = _initializers();

        for (auto pair : initializers)
        {
            auto record = pair.second;

            if (record.mFnInitialize())
            {
                UninitializeFunction uninitializer = record.mFnUninitialize;
                if (uninitializer != NULL)
                    uninitializers.push_back(uninitializer);
            }
        }

        // This data is useless anymore - not a big deal but let clear it
        initializers.clear();
    }

    void uninitialize_objects()
    {
        // uninitialize all global objects
        for (std::vector<UninitializeFunction>::reverse_iterator it = uninitializers.rbegin();
             it != uninitializers.rend();
             ++it)
        {
            UninitializeFunction& uninitializer = *it;
            uninitializer();
        }
    }

    static Initializers& _initializers()
    {
        static Initializers initializers;
        return initializers;
    }

    std::vector<UninitializeFunction> uninitializers;
};

template <>
inline bool InitializationManager::subscribe(int rank,
                                             InitializeFunction fnInitializer,
                                             UninitializeFunction fnUninitializer)
{
    add(rank, fnInitializer, fnUninitializer);
    return true;
}
}
