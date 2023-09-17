#ifndef LAB5_SLINK_H
#define LAB5_SLINK_H

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SLink {
    SLink(const std::string& name, const TStFileVariant& linkTo, const TStDirectory& parentDir);

    SFileSystemInfo Info;
    TWFileVariant LinkTo;
    TWDirectory ParentDir;
};


#endif //LAB5_SLINK_H
