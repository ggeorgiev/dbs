//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "parser/tokenizer.hpp"
#include "dom/cxx/cxx_program.hpp"
#include "dom/manager.h"
#include "doim/fs/fs_file.hpp"
#include "doim/generic/object.hpp"
#include "doim/manager.h"
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

    ECode initialize(const StreamSPtr& stream, const doim::FsFileSPtr& location)
    {
        EHAssert(stream != nullptr);
        EHAssert(location != nullptr);

        mLocation = location;
        mTokenizer = std::make_shared<Tokenizer>();
        EHTest(mTokenizer->initialize(stream));
        EHEnd;
    }

    dom::CxxProgramSPtr cxxProgram() const
    {
        return mCxxProgram;
    }

    ECode parse()
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.none())
                break;

            if (type.test(Token::kKeywordCxxLibrary))
            {
                EHTest(parseCxxLibrary());
                continue;
            }

            if (type.test(Token::kKeywordCxxProgram))
            {
                EHTest(parseCxxProgram());
                continue;
            }
        }

        EHEnd;
    }

    ECode parseCxxLibrary()
    {
        auto type = nextMeaningfulToken();

        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type);

        auto name = mTokenizer->token();
        auto object = doim::gManager->obtainObject(mLocation->directory(),
                                                   doim::Object::Type::kCxxLibrary,
                                                   name);
        auto library = dom::gManager->obtainCxxLibrary(object);

        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorColon))
                break;

            if (type.test(Token::kOperatorAt))
            {
                dom::Attribute attribute;
                EHTest(parseAttribute(attribute));
                EHTest(library->updateAttribute(attribute));
                continue;
            }

            EHBan(kUnable);
        }

        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kKeywordCxxPublicHeader))
            {
                auto type = nextMeaningfulToken();

                auto directory = mLocation->directory();
                if (type.test(Token::kOperatorAt))
                {
                    dom::Attribute attribute;
                    EHTest(parseAttribute(attribute));
                    if (attribute.mName == "directory")
                    {
                        directory =
                            doim::gManager->obtainDirectory(directory, attribute.mValue);
                    }

                    type = nextMeaningfulToken();
                }

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<doim::FsFileSPtr> files;
                EHTest(parseFiles(directory, std::numeric_limits<size_t>::max(), files));
                EHTest(library->updateCxxPublicHeaders(directory, files));

                continue;
            }

            if (type.test(Token::kKeywordCxxBinary))
            {
                auto type = nextMeaningfulToken();

                std::unordered_set<doim::FsFileSPtr> files;
                EHTest(parseFiles(mLocation->directory(), 1, files));

                if (files.size() < 1)
                    EHBan(kUnable);

                EHTest(library->updateBinary(*files.begin()));

                continue;
            }

            if (type.test(Token::kKeywordCxxLibrary))
            {
                auto type = nextMeaningfulToken();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<doim::ObjectSPtr> objects;
                EHTest(parseObjects(mLocation->directory(),
                                    doim::Object::Type::kCxxLibrary,
                                    objects));

                std::unordered_set<dom::CxxLibrarySPtr> libraries;
                for (const auto& object : objects)
                    libraries.insert(dom::gManager->obtainCxxLibrary(object));

                EHTest(library->updateCxxLibraries(libraries));

                continue;
            }

            if (type.test(Token::kKeywordCxxFile))
            {
                auto type = nextMeaningfulToken();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<doim::FsFileSPtr> files;
                EHTest(parseFiles(mLocation->directory(),
                                  std::numeric_limits<size_t>::max(),
                                  files));
                EHTest(library->updateCxxFilesList(files));

                continue;
            }

            EHBan(kUnable, type, mTokenizer->token());
        }

        EHEnd;
    }

    ECode parseCxxProgram()
    {
        auto type = nextMeaningfulToken();

        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type);

        type = nextMeaningfulToken();
        if (!type.test(Token::kOperatorColon))
            EHBan(kUnable);

        mCxxProgram = std::make_shared<dom::CxxProgram>();

        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kKeywordCxxFile))
            {
                auto type = nextMeaningfulToken();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<doim::FsFileSPtr> files;
                EHTest(parseFiles(mLocation->directory(),
                                  std::numeric_limits<size_t>::max(),
                                  files));
                EHTest(mCxxProgram->updateCxxFilesList(files));

                continue;
            }

            if (type.test(Token::kKeywordCxxLibrary))
            {
                auto type = nextMeaningfulToken();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<doim::ObjectSPtr> objects;
                EHTest(parseObjects(mLocation->directory(),
                                    doim::Object::Type::kCxxLibrary,
                                    objects));

                std::unordered_set<dom::CxxLibrarySPtr> libraries;
                for (const auto& object : objects)
                    libraries.insert(dom::gManager->obtainCxxLibrary(object));

                EHTest(mCxxProgram->updateCxxLibraries(libraries));

                continue;
            }

            EHBan(kUnable);
        }

        EHEnd;
    }

    ECode parseObjects(const doim::LocationSPtr& location,
                       const doim::Object::Type objectType,
                       std::unordered_set<doim::ObjectSPtr>& objects)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                auto token = mTokenizer->token();
                auto object = doim::gManager->obtainObject(location, objectType, token);
                objects.insert(object);
                continue;
            }

            EHBan(kUnable);
        }
        EHEnd;
    }

    ECode parseDirectories(const doim::FsDirectorySPtr& directory,
                           const size_t limit,
                           std::unordered_set<doim::FsDirectorySPtr>& directories)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                const auto& token = mTokenizer->token();
                const auto& dir = doim::gManager->obtainDirectory(directory, token);
                directories.insert(dir);

                if (directories.size() > limit)
                    EHBan(kUnable);

                continue;
            }

            EHBan(kUnable);
        }
        EHEnd;
    }

    ECode parseFiles(const doim::FsDirectorySPtr& directory,
                     const size_t limit,
                     std::unordered_set<doim::FsFileSPtr>& files)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                const auto& token = mTokenizer->token();
                const auto& file = doim::gManager->obtainFile(directory, token);
                files.insert(file);

                if (files.size() > limit)
                    EHBan(kUnable);

                continue;
            }

            EHBan(kUnable);
        }
        EHEnd;
    }

    ECode parseAttribute(dom::Attribute& attribute)
    {
        auto type = nextMeaningfulToken();

        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type, mTokenizer->token());

        attribute.mName = mTokenizer->token();

        type = nextMeaningfulToken();
        if (!type.test(Token::kOperatorAssignment))
            EHBan(kUnable, type, mTokenizer->token());

        type = nextMeaningfulToken();
        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type, mTokenizer->token());

        attribute.mValue = mTokenizer->token();

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
    doim::FsFileSPtr mLocation;

    dom::CxxProgramSPtr mCxxProgram;
    TokenizerSPtr mTokenizer;
};
}
