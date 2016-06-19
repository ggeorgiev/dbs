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
}

ECode Depository::updateUrl(const doim::UrlSPtr& url)
{
    mUrl = url;
}
}
