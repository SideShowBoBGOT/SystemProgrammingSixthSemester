#include <ranges>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <numeric>

#include "TFileSystem.h"
#include "../Models/SDirectory.h"
#include "../Models/SLink.h"
#include "../Models/SFile.h"
#include "../Helpers/SOverloadVariant.h"
#include "../Models/Errors/TPathException.h"

const static constexpr std::string_view s_sRootPath = "/";
const static constexpr std::string_view s_sSelfPath = "";
const TStDirectory TFileSystem::s_pRootDir = std::make_shared<SDirectory>("", nullptr);

void ff(const char* p) {
    ;
}

static void ModifyAttr(const auto& fileObject, struct stat *st) {
    st->st_uid = fileObject->Info.Uid;
    st->st_gid = fileObject->Info.Gid;
    st->st_atime = fileObject->Info.Atime;
    st->st_mtime = fileObject->Info.Mtime;
    st->st_mode = fileObject->Info.Mode;
}

int TFileSystem::GetAttr(const char *path, struct stat *st) {
    const auto result = Find(path);
    if(!result) {
        std::visit([](const auto& ex) { std::cout << ex.what() << "\n"; }, result.error());
        return -ENOENT;
    }
    std::visit(SOverloadVariant{
        [st](const TStDirectory& dir) {
            ModifyAttr(dir, st);
            st->st_nlink = static_cast<nlink_t>(dir->FileVariants.size());
        },
        [st](const TStFile& file) {
            ModifyAttr(file, st);
            st->st_nlink = 1;
            st->st_size = static_cast<off_t>(file->Content.size());
        },
        [st](const TStLink& link) {
            ModifyAttr(link, st);
            st->st_nlink = 1;
            //st->st_size = static_cast<off_t>(link->LinkTo.size());
        }
    }, result.value());
    return 0;
}

int TFileSystem::SymLink(const char *target_path, const char *link_path) {
    const auto result = Find(target_path);
    if(!result) {
        std::visit([](const auto& ex) { std::cout << ex.what() << "\n"; }, result.error());
        return -ENOENT;
    }
    return 0;
}

int TFileSystem::MkDir(const char *path, mode_t mode) {
    return 0;
}

int
TFileSystem::ReadDir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    return 0;
}

int TFileSystem::MkNod(const char *path, mode_t mode, dev_t rdev) {
    return 0;
}

int TFileSystem::Read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    return 0;
}

int TFileSystem::Write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info) {
    return 0;
}

int TFileSystem::ChMod(const char *path, mode_t mode) {
    return 0;
}

int TFileSystem::ReadLink(const char *path, char *buffer, size_t size) {
    return 0;
}

std::expected<TStFileVariant, TFileSystemVariantException> TFileSystem::Find(const std::filesystem::path& path) {
    if(path.empty() or std::string_view(path.begin()->c_str())!=s_sRootPath) {
        return std::unexpected(TFileObjectNotExistException(path.begin(), path.end()));
    }

    auto tempDir = s_pRootDir;
    for(auto it = ++path.begin();it!=path.end();++it) {
        const auto nameStr = std::string_view(it->c_str());
        if(nameStr == s_sSelfPath) {
            return tempDir;
        }
        const auto objIt = std::ranges::find_if(tempDir->FileVariants,
            [&nameStr](const TStFileVariant& var) {
                return std::visit([&nameStr](const auto& obj) {
                    return obj->Info.Name == nameStr;
                }, var);
            }
        );
        if(objIt == tempDir->FileVariants.end()) {
            return std::unexpected(TFileObjectNotExistException(path.begin(), it));
        }
        if(std::distance(it, path.end()) == 1) {
            return *objIt;
        }
        if(const auto t = std::get_if<TStDirectory>(&*objIt)) {
            tempDir = *t;
            continue;
        }
        return std::unexpected(TFileObjectNotExistException(path.begin(), it));
    }

    return tempDir;
}
