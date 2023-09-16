#include "SFile.h"

SFile::SFile(const SFileSystemInfo& info, const not_null_shared_ptr<SDirectory>& parentDir)
    : Info{info}, ParentDir{parentDir.get()} {}
