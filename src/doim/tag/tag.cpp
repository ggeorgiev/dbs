//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "doim/tag/tag.h"
#include <functional>

namespace doim
{
TagSPtr gTaskTag = Tag::global("task", gTaskTag);

TagSPtr gCrcTag = Tag::global("crc", gCrcTag);
TagSPtr gDependTag = Tag::global("depend", gDependTag);

TagSPtr gDbTag = Tag::global("db", gDbTag);
TagSPtr gParseTag = Tag::global("parse", gParseTag);
TagSPtr gSysTag = Tag::global("sys", gSysTag);

TagSPtr gRunTag = Tag::global("run", gRunTag);
TagSPtr gDoneTag = Tag::global("done", gDoneTag);

TagSPtr gLoadTag = Tag::global("load", gLoadTag);
TagSPtr gSaveTag = Tag::global("save", gSaveTag);

TagSPtr gCxxTag = Tag::global("cxx", gCxxTag);
TagSPtr gObjTag = Tag::global("obj", gObjTag);
TagSPtr gProgramTag = Tag::global("program", gProgramTag);
TagSPtr gProtobufTag = Tag::global("protobuf", gProtobufTag);
}
