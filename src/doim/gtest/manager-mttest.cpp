//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#include "doim/manager.h"
#include "doim/fs/fs_directory.h"
#include "doim/tag/tag.h"
#include "gtest/framework.h"
#include <boost/thread/barrier.hpp>
#include <memory>
#include <thread>
#include <unordered>
#include <vector>
#include <stddef.h>

TEST(ManagerTest, mt_unique)
{
    size_t count = 10;

    boost::barrier barrier(count);
    std::vector<std::shared_ptr<std::thread>> threads;

    std::unique_ptr<doim::TagSPtr[]> tags(new doim::TagSPtr[count]);

    for (size_t i = 0; i < count; ++i)
    {
        auto thread = std::make_shared<std::thread>([&barrier, &tags, i]() {
            auto tag = doim::gManager->unique(doim::Tag::make("foo"));

            barrier.wait();
            tags[i] = doim::gManager->unique(tag);
        });
        threads.push_back(thread);
    }
    for (const auto& thread : threads)
        thread->join();

    for (size_t i = 1; i < count; ++i)
        ASSERT_EQ(tags[0], tags[i]);
}

TEST(ManagerTest, mt_unique_set)
{
    size_t count = 10;

    boost::barrier barrier(count);
    std::vector<std::shared_ptr<std::thread>> threads;

    std::unique_ptr<doim::TagSetSPtr[]> tagSets(new doim::TagSetSPtr[count]);

    for (size_t i = 0; i < count; ++i)
    {
        auto thread = std::make_shared<std::thread>([&barrier, &tagSets, i]() {
            auto tag = doim::gManager->unique(doim::Tag::make("foo"));
            auto tagSet = doim::TagSet::make();
            tagSet->insert(tag);

            barrier.wait();
            tagSets[i] = doim::gManager->unique(tagSet);

        });
        threads.push_back(thread);
    }
    for (const auto& thread : threads)
        thread->join();

    for (size_t i = 1; i < count; ++i)
        ASSERT_EQ(tagSets[0], tagSets[i]);
}

TEST(ManagerTest, mt_addSameObject)
{
    size_t count = 10;

    boost::barrier barrier(count);
    std::vector<std::shared_ptr<std::thread>> threads;

    std::unique_ptr<doim::FsDirectorySPtr[]> directories(
        new doim::FsDirectorySPtr[count]);

    for (size_t i = 0; i < count; ++i)
    {
        auto thread = std::make_shared<std::thread>([&barrier, &directories, i]() {
            barrier.wait();
            directories[i] = doim::gManager->obtainDirectory(nullptr, "/");
        });
        threads.push_back(thread);
    }
    for (const auto& thread : threads)
        thread->join();

    for (size_t i = 1; i < count; ++i)
        ASSERT_EQ(directories[0], directories[i]);
}
