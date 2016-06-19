//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/element.hpp"
#include "doim/url/url.h"
#include "err/err.h"
#include <memory>

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

    doim::UrlSPtr gitUrl()
    {
        return mGitUrl;
    }

    // Updates
    ECode updateType(const EType& type);

    ECode updateGitUrl(const doim::UrlSPtr& url);

private:
    EType mType;
    doim::UrlSPtr mGitUrl;
};
}
