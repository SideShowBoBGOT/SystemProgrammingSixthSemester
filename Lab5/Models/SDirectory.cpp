#include "SDirectory.h"
#include "SFile.h"
#include "SLink.h"

SDirectory::SDirectory(const SFileSystemInfo& info, const TNullableDirectory& parentDir)
    : Info{info}, ParentDir{parentDir} {}