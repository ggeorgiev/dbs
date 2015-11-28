//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "parser/tokenizer.hpp"

#include "dom/fs/fs_manager.h"
#include "dom/fs/fs_file.hpp"

#include "err/err.h"

#include <memory>
#include <unordered_set>

namespace parser
{
template <typename S>
class Parser
{
public:
    typedef S Stream;
    typedef std::shared_ptr<Stream> StreamSPtr;

    typedef Tokenizer<Stream> Tokenizer;
    typedef std::shared_ptr<Tokenizer> TokenizerSPtr;

    ECode initialize(const StreamSPtr& stream)
    {
        EHAssert(stream != nullptr);
        mTokenizer = std::make_shared<Tokenizer>();
        EHTest(mTokenizer->initialize(stream));
        EHEnd;
    }

    ECode parseFiles(std::unordered_set<dom::FsFileSPtr>& files) const
    {
        auto root = dom::gFsManager->obtainDirectory(nullptr, "/");
        for (;;)
        {
            auto type = mTokenizer->next();
            if (type == TokenType::kNil)
                break;

            if ((type & TokenType::kPath) != TokenType::kNil)
            {
                auto token = mTokenizer->token();
                auto file = dom::gFsManager->obtainFile(root, token);
                files.emplace(file);
            }
        }
        EHEnd;
    }

private:
    TokenizerSPtr mTokenizer;
};
}
