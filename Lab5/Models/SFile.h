#ifndef LAB5_SFILE_H
#define LAB5_SFILE_H

#include <vector>

#include "SFileSystemInfo.h"
#include "TCommonDeclarations.h"

struct SDirectory;

struct SFile : std::enable_shared_from_this<SFile> {
    SFile(const std::string& name, const TStDirectory& parentDir);

    SFileSystemInfo Info;
    std::vector<char> Content;
    TWDirectory ParentDir;
};


#endif //LAB5_SFILE_H
