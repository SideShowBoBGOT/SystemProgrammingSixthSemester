#include <ctime>
#include <unistd.h>

#include "SFileSystemInfo.h"

SFileSystemInfo::SFileSystemInfo(const std::string& name)
    : Name{name}, Uid{getuid()}, Gid{getgid()}, Atime{time(nullptr)}, Mtime{time(nullptr)}, Mode{0777} {}
