//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "doim/element.hpp"

namespace doim
{
template <typename T, typename TN, typename... Args>
class TreeLeaf : public Element<T, TN, Args...>
{
public:
    using Element<T, TN, Args...>::Element;
};
}
