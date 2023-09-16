#ifndef LAB5_SLINK_H
#define LAB5_SLINK_H

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SLink {
    SLink(const std::string& name, const std::string& linkTo, const TNullableDirectory& parentDir);

    SFileSystemInfo Info;
    std::string LinkTo;
    TWeakDirectory ParentDir;
};


#endif //LAB5_SLINK_H
