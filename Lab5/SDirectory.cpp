#include "SDirectory.h"

SDirectory::SDirectory(const SFileSystemInfo& info, const PDirectory& parentDir)
    : Info{info}, ParentDir{parentDir.get()} {}
