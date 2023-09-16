#ifndef LAB5_SFILE_H
#define LAB5_SFILE_H

#include <vector>

#include "SFileSystemInfo.h"
#include "TCommonDeclarations.h"

class SDirectory;

struct SFile {
    SFile(const SFileSystemInfo& info, const PDirectory& parentDir);

    SFileSystemInfo Info;
    std::vector<std::byte> Content;
    WDirectory ParentDir;
};


#endif //LAB5_SFILE_H
