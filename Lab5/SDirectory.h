#ifndef LAB5_SDIRECTORY_H
#define LAB5_SDIRECTORY_H

#include <vector>

#include <gsl/gsl>

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SDirectory {
    SDirectory(const SFileSystemInfo& info, const PDirectory& parentDir);

    SFileSystemInfo Info;
    WDirectory ParentDir;
    std::vector<PDirectory> SubDirs;
    std::vector<PLink> Links;
    std::vector<PFile> Files;
};


#endif //LAB5_SDIRECTORY_H
