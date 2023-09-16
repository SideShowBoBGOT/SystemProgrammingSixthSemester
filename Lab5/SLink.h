#ifndef LAB5_SLINK_H
#define LAB5_SLINK_H

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SFile;
struct SDirectory;

struct SLink {
    SLink(const SFileSystemInfo& info, const TWeakFileVariant& fileVariant, const TStrongDirectory& parentDir);

    SFileSystemInfo Info;
    TWeakFileVariant FileVariant;
    TWeakDirectory ParentDir;
};


#endif //LAB5_SLINK_H
