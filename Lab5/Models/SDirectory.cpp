#include <fcntl.h>

#include "SDirectory.h"
#include "SFile.h"
#include "SLink.h"

SDirectory::SDirectory(const std::string& name, const TOptStDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, ParentDir{parentDir} {
    Info.Mode = S_IFDIR | 0777;
}