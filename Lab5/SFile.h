#ifndef LAB5_SFILE_H
#define LAB5_SFILE_H

#include <vector>

#include "SFileSystemInfo.h"
#include "TFileVariant.h"

class SDirectory;

struct SFile {
    SFile(const SFileSystemInfo& info, const PDirectory& parentDir);

    SFileSystemInfo Info;
    std::vector<std::byte> Content;
    PDirectory ParentDir;
};


#endif //LAB5_SFILE_H
