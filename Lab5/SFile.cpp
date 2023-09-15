//
// Created by sideshowbobgot on 15.09.23.
//

#include "SFile.h"

SFile::SFile(const SFileSystemInfo& info, const not_null_shared_ptr<SDirectory>& parentDir)
    : Info{info}, ParentDir{parentDir} {}
