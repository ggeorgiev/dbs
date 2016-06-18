//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_file.hpp"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <axe.h> // IWYU pragma: keep

namespace parser
{
static auto r_annexKw = r_str("annex");

struct Annex
{
    Annex(std::vector<string>& errors, File& file)
        : mErrors(errors)
        , mFile(file)
    {
    }

    void operator()(I& i1, I& i2)
    {
        DbsParser parser;
        ECode code = parser.parse(mFile.mFile);
        if (code != err::kSuccess)
        {
            auto error = err::gError.release();
            mErrors.push_back(error->message());
        }
    };

    File& mFile;
    std::vector<string>& mErrors;
};
}
