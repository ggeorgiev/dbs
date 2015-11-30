//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "parser/tokenizer.hpp"

#include "dom/cpp/cpp_program.hpp"

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

    typedef typename Tokenizer::Token Token;

    ECode initialize(const StreamSPtr& stream, const dom::FsFileSPtr& location)
    {
        EHAssert(stream != nullptr);
        EHAssert(location != nullptr);

        mLocation = location;
        mTokenizer = std::make_shared<Tokenizer>();
        EHTest(mTokenizer->initialize(stream));
        EHEnd;
    }

    dom::CppProgramSPtr cppProgram() const { return mCppProgram; }
    ECode parse()
    {
        for (;;)
        {
            auto type = mTokenizer->next();

            if (type.none())
                break;

            if (type.test(Token::kWhiteSpace))
                continue;

            if (type.test(Token::kKeywordCppProgram))
            {
                EHTest(parseCppProgram());
                continue;
            }
        }

        EHEnd;
    }

    ECode parseCppProgram()
    {
        auto type = nextMeaningfulToken();

        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type);

        type = nextMeaningfulToken();
        if (!type.test(Token::kOperatorColon))
            EHBan(kUnable);

        mCppProgram = std::make_shared<dom::CppProgram>();

        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kWhiteSpace))
                continue;

            if (type.test(Token::kKeywordCppFile))
            {
                type = mTokenizer->next();
                if (type.test(Token::kWhiteSpace))
                    type = mTokenizer->next();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<dom::FsFileSPtr> files;
                EHTest(parseFiles(mLocation->directory(), files));
                EHTest(mCppProgram->updateCppFiles(files));

                continue;
            }

            EHBan(kUnable);
        }

        EHEnd;
    }

    ECode parseFiles(const dom::FsDirectorySPtr& directory,
                     std::unordered_set<dom::FsFileSPtr>& files)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                auto token = mTokenizer->token();
                auto file = dom::gFsManager->obtainFile(directory, token);
                files.emplace(file);
                continue;
            }

            EHBan(kUnable);
        }
        EHEnd;
    }

    typename Token::Type nextMeaningfulToken()
    {
        typename Token::Type type;
        do
        {
            type = mTokenizer->next();
        } while (type.test(Token::kWhiteSpace) || type.test(Token::kComment));
        return type;
    }

private:
    dom::FsFileSPtr mLocation;

    dom::CppProgramSPtr mCppProgram;
    TokenizerSPtr mTokenizer;
};
}
