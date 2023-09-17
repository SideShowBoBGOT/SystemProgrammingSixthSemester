#ifndef LAB5_SLINK_H
#define LAB5_SLINK_H

#include <filesystem>

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SLink {
    SLink(const std::string& name, const std::filesystem::path& linkTo, const TStDirectory& parentDir);

    SFileSystemInfo Info;
    std::filesystem::path LinkTo;
    TWDirectory ParentDir;
};


#endif //LAB5_SLINK_H
