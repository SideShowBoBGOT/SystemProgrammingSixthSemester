//
// Created by sideshowbobgot on 15.09.23.
//

#include "SFileSystemInfo.h"

SFileSystemInfo::SFileSystemInfo(const std::string& name, uid_t uid, gid_t gid, time_t atime, time_t mtime, mode_t mode)
    : Name{name}, Uid{uid}, Gid{gid}, Atime{atime}, Mtime{mtime}, Mode{mode} {}
