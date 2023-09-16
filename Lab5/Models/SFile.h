#ifndef LAB5_SFILE_H
#define LAB5_SFILE_H

#include <vector>

#include "SFileSystemInfo.h"
#include "TCommonDeclarations.h"

struct SDirectory;

struct SFile {
    SFile(const std::string& name, const TNullableDirectory& parentDir);

    SFileSystemInfo Info;
    std::vector<std::byte> Content;
    TWeakDirectory ParentDir;
};


#endif //LAB5_SFILE_H
