#include <fcntl.h>
#include "SFile.h"

SFile::SFile(const std::string& name, const TOptStDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, ParentDir{parentDir} {
    Info.Mode = S_IFREG | 0777;
}
