//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include "parser/dbs/e_file.hpp"
#include "parser/dbs/e_particle.hpp"
#include "doim/fs/fs_file.h"
#include "err/err.h"
#include <axe.h> // IWYU pragma: keep

namespace parser
{
static auto r_annexKw = r_str("annex");

struct Annex
{
    Annex(const doim::FsDirectorySPtr& location, std::vector<string>& errors)
        : mErrors(errors)
        , mFile(location)
    {
    }

    auto parse()
    {
        return e_ref([this](I& i1, I& i2) {
            DbsParser parser;
            ECode code = parser.parse(mFile.mFile);
            if (code != err::kSuccess)
            {
                auto error = err::gError.release();
                mErrors.push_back(error->message());
            }
        });
    }

    template <typename WS>
    auto rule(const WS& r_ws)
    {
        return r_ws & r_annexKw & r_ws & r_colon & mFile.mDirectory.r_reset() &
               *(mFile.rule(r_ws) >> parse()) & r_ws & r_semicolon;
    }

    File mFile;
    std::vector<string>& mErrors;
};
}
