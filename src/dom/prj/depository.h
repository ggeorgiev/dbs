//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/element.hpp"
#include "doim/url/url.h"
#include "err/err.h"
#include <memory>
#include <str>
#include <tuple>

namespace dom
{
class Depository;
typedef shared_ptr<Depository> DepositorySPtr;

class Depository : public Element<Depository>
{
public:
    enum class EType
    {
        kGitRepository,
    };

    Depository();

    // Accessors
    EType type()
    {
        return mType;
    }

    doim::UrlSPtr url()
    {
        return mUrl;
    }

    // Updates
    ECode updateType(const EType& type);

    ECode updateUrl(const doim::UrlSPtr& url);

private:
    EType mType;
    doim::UrlSPtr mUrl;
};
}
