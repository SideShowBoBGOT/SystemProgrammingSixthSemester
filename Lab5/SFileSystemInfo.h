#ifndef LAB5_SFILESYSTEMINFO_H
#define LAB5_SFILESYSTEMINFO_H

#include <string>
#include <vector>

struct SFileSystemInfo {
    SFileSystemInfo()=default;
    SFileSystemInfo(const std::string& name, uid_t uid, gid_t gid, time_t atime, time_t mtime, mode_t mode);

    std::string Name{};
    uid_t Uid{};
    gid_t Gid{};
    time_t Atime{};
    time_t Mtime{};
    mode_t Mode{};
};


#endif //LAB5_SFILESYSTEMINFO_H
