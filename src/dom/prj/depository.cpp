//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "dom/prj/depository.h"

namespace dom
{
Depository::Depository()
{
}

ECode Depository::updateType(const EType& type)
{
    mType = type;
    EHEnd;
}

ECode Depository::updateGitUrl(const doim::UrlSPtr& url)
{
    mGitUrl = url;
    EHEnd;
}
}
