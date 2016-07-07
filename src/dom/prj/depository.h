//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "dom/element.hpp"
#include "doim/generic/object.h"
#include "doim/url/url.h"
#include "err/err.h"
#include <memory>
#include <str>

namespace dom
{
class Depository;
typedef shared_ptr<Depository> DepositorySPtr;

class Depository : public Element<Depository>
{
public:
    static doim::Object::EType objectType()
    {
        return doim::Object::EType::kDepository;
    }

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

    string gitTag()
    {
        return mGitTag;
    }

    // Updates
    ECode updateType(const EType& type);

    ECode updateGitUrl(const doim::UrlSPtr& url);

    ECode updateGitTag(const std::string& tag);

private:
    EType mType;
    doim::UrlSPtr mGitUrl;
    string mGitTag;
};
}
