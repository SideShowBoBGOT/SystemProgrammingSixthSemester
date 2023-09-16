#include "SDirectory.h"
#include "SFile.h"
#include "SLink.h"

SDirectory::SDirectory(const SFileSystemInfo& info, const TStrongDirectory& parentDir)
    : Info{info}, ParentDir{parentDir.get()} {}