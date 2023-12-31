#include <fcntl.h>
#include "SFile.h"

SFile::SFile(const std::string& name, const TStDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, ParentDir{parentDir.get()} {
    Info.Mode = S_IFREG | 0777;
}
