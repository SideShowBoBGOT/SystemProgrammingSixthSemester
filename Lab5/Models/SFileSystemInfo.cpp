#include <ctime>
#include <unistd.h>
#include <fcntl.h>

#include "SFileSystemInfo.h"

SFileSystemInfo::SFileSystemInfo(const std::string& name)
    : Name{name}, Uid{getuid()}, Gid{getgid()}, Atime{time(nullptr)}, Mtime{time(nullptr)}, Mode{S_IFDIR | 0777} {}
