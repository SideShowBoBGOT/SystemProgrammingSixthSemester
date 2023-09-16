#include "SFile.h"

SFile::SFile(const SFileSystemInfo& info, const TNullableDirectory& parentDir)
    : Info{info}, ParentDir{parentDir} {}
