//  Copyright © 2015 George Georgiev. All rights reserved.
//

#include "parser/tokenizer.hpp"

#include "dom/generic/object.hpp"
#include "dom/generic/manager.h"

#include "dom/cpp/cpp_program.hpp"
#include "dom/cpp/cpp_manager.h"

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
            auto type = nextMeaningfulToken();

            if (type.none())
                break;

            if (type.test(Token::kKeywordCppLibrary))
            {
                EHTest(parseCppLibrary());
                continue;
            }

            if (type.test(Token::kKeywordCppProgram))
            {
                EHTest(parseCppProgram());
                continue;
            }
        }

        EHEnd;
    }

    ECode parseCppLibrary()
    {
        auto type = nextMeaningfulToken();

        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type);

        auto name = mTokenizer->token();
        auto object = dom::gManager->obtainObject(mLocation->directory(),
                                                  dom::Object::Type::kCppLibrary,
                                                  name);
        auto library = dom::gCppManager->obtainCppLibrary(object);

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

            if (type.test(Token::kKeywordCppPublicDirectory))
            {
                auto type = nextMeaningfulToken();

                std::unordered_set<dom::FsDirectorySPtr> directories;
                EHTest(parseDirectories(mLocation->directory(), 1, directories));

                if (directories.size() < 1)
                    EHBan(kUnable);

                EHTest(library->updatePublicHeadersDirectory(*directories.begin()));

                continue;
            }

            if (type.test(Token::kKeywordCppBinary))
            {
                auto type = nextMeaningfulToken();

                std::unordered_set<dom::FsFileSPtr> files;
                EHTest(parseFiles(mLocation->directory(), 1, files));

                if (files.size() < 1)
                    EHBan(kUnable);

                EHTest(library->updateBinary(*files.begin()));

                continue;
            }

            EHBan(kUnable, type, mTokenizer->token());
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

            if (type.test(Token::kKeywordCppFile))
            {
                auto type = nextMeaningfulToken();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<dom::FsFileSPtr> files;
                EHTest(
                    parseFiles(mLocation->directory(), std::numeric_limits<size_t>::max(), files));
                EHTest(mCppProgram->updateCppFiles(files));

                continue;
            }

            if (type.test(Token::kKeywordCppLibrary))
            {
                auto type = nextMeaningfulToken();

                if (!type.test(Token::kOperatorColon))
                    EHBan(kUnable);

                std::unordered_set<dom::ObjectSPtr> objects;
                EHTest(
                    parseObjects(mLocation->directory(), dom::Object::Type::kCppLibrary, objects));

                std::unordered_set<dom::CppLibrarySPtr> libraries;
                for (const auto& object : objects)
                    libraries.emplace(dom::gCppManager->obtainCppLibrary(object));

                EHTest(mCppProgram->updateCppLibraries(libraries));

                continue;
            }

            EHBan(kUnable);
        }

        EHEnd;
    }

    ECode parseObjects(const dom::LocationSPtr& location,
                       const dom::Object::Type objectType,
                       std::unordered_set<dom::ObjectSPtr>& objects)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                auto token = mTokenizer->token();
                auto object = dom::gManager->obtainObject(location, objectType, token);
                objects.emplace(object);
                continue;
            }

            EHBan(kUnable);
        }
        EHEnd;
    }

    ECode parseDirectories(const dom::FsDirectorySPtr& directory,
                           const size_t limit,
                           std::unordered_set<dom::FsDirectorySPtr>& directories)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                const auto& token = mTokenizer->token();
                const auto& dir = dom::gFsManager->obtainDirectory(directory, token);
                directories.emplace(dir);

                if (directories.size() > limit)
                    EHBan(kUnable);

                continue;
            }

            EHBan(kUnable);
        }
        EHEnd;
    }

    ECode parseFiles(const dom::FsDirectorySPtr& directory,
                     const size_t limit,
                     std::unordered_set<dom::FsFileSPtr>& files)
    {
        for (;;)
        {
            auto type = nextMeaningfulToken();

            if (type.test(Token::kOperatorSemicolon))
                break;

            if (type.test(Token::kPath))
            {
                const auto& token = mTokenizer->token();
                const auto& file = dom::gFsManager->obtainFile(directory, token);
                files.emplace(file);

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
            EHBan(kUnable, type);

        attribute.mName = mTokenizer->token();

        type = nextMeaningfulToken();
        if (!type.test(Token::kIdentifier))
            EHBan(kUnable, type);

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
    dom::FsFileSPtr mLocation;

    dom::CppProgramSPtr mCppProgram;
    TokenizerSPtr mTokenizer;
};
}
