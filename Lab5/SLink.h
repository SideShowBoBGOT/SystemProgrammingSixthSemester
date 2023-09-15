#ifndef LAB5_SLINK_H
#define LAB5_SLINK_H

#include "TFileVariant.h"
#include "SFileSystemInfo.h"

class SFile;
class SDirectory;

struct SLink {
    SLink(const SFileSystemInfo& info, const TFileVariant& fileVariant, const PDirectory& parentDir);

    SFileSystemInfo Info;
    TFileVariant FileVariant;
    PDirectory ParentDir;
};


#endif //LAB5_SLINK_H
