//  Copyright © 2015-2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/element_manager.hpp"
#include "im/initialization_manager.hpp"
#include <boost/functional/hash.hpp>
#include <memory>
#include <shared_ptr>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dom
{
template <typename T>
class Element : public enable_make_shared<T>
{
public:
    static shared_ptr<T> obtain(const doim::ObjectSPtr& object)
    {
        return gElementManager->obtain(object);
    }

    static shared_ptr<T> find(const doim::ObjectSPtr& object)
    {
        return gElementManager->find(object);
    }

    Element()
    {
    }

protected:
    static shared_ptr<ElementManager<T>> gElementManager;
};

template <typename T>
shared_ptr<ElementManager<T>> Element<T>::gElementManager =
    im::InitializationManager::subscribe(gElementManager);
}
