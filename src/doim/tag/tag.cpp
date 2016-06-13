//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag.h"
#include <functional>

namespace doim
{
TagSPtr gCrcTag = Tag::global("crc", gCrcTag);
TagSPtr gInspectTag = Tag::global("inspect", gInspectTag);

TagSPtr gDbTag = Tag::global("db", gDbTag);
TagSPtr gParseTag = Tag::global("parse", gParseTag);
TagSPtr gSysTag = Tag::global("sys", gSysTag);

TagSPtr gTaskTag = Tag::global("task", gTaskTag);
TagSPtr gRunTag = Tag::global("run", gRunTag);
TagSPtr gDoneTag = Tag::global("done", gDoneTag);

doim::TagSetSPtr gRunTagSet =
    doim::TagSet::global({&doim::gTaskTag, &doim::gRunTag}, gRunTagSet);
doim::TagSetSPtr gDoneTagSet =
    doim::TagSet::global({&doim::gTaskTag, &doim::gDoneTag}, gDoneTagSet);

TagSPtr gLoadTag = Tag::global("load", gLoadTag);
TagSPtr gSaveTag = Tag::global("save", gSaveTag);

TagSPtr gCxxTag = Tag::global("cxx", gCxxTag);
TagSPtr gObjTag = Tag::global("obj", gObjTag);
TagSPtr gProgramTag = Tag::global("program", gProgramTag);
TagSPtr gProtobufTag = Tag::global("protobuf", gProtobufTag);
}
