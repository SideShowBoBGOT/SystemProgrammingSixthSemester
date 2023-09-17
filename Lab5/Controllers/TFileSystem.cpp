#include <ranges>
#include <filesystem>

#include "TFileSystem.h"
#include "../Models/SDirectory.h"
#include "../Models/SLink.h"
#include "../Models/SFile.h"
#include "../Models/Errors/TPathException.h"

const static constexpr std::string_view s_sRootPath = "/";
const static constexpr std::string_view s_sSelfPath = "";
const static constexpr std::string_view s_sCurrentPath = ".";
const static constexpr std::string_view s_sParentPath = "..";

const TStDirectory TFileSystem::s_pRootDir = std::make_shared<SDirectory>("", nullptr);

static void ModifyAttr(const auto& fileObject, struct stat *st) {
    st->st_uid = fileObject->Info.Uid;
    st->st_gid = fileObject->Info.Gid;
    st->st_atime = fileObject->Info.Atime;
    st->st_mtime = fileObject->Info.Mtime;
    st->st_mode = fileObject->Info.Mode;
}

int TFileSystem::GetAttr(const char *path, struct stat *st, struct fuse_file_info *fi) {
    const auto result = Find(path);
    if(!result) return PrintErrGetVal(result);

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

int TFileSystem::ReadLink(const char *path, char *buffer, size_t size) {
    const auto linkRes = FindLink(path);
    if(!linkRes) PrintErrGetVal(linkRes);
    const auto& link = linkRes.value();
    memcpy(buffer, link->LinkTo.c_str(), size);
    return 0;
}

int TFileSystem::MkNod(const char *path, mode_t mode, dev_t rdev) {
    const auto newDirPath = std::filesystem::path(path);
    auto parentDirRes = FindDir(newDirPath.parent_path());
    if(!parentDirRes) return PrintErrGetVal(parentDirRes);
    auto& parentDir = parentDirRes.value();
    const auto file = parentDir->MakeFileObject<SFile>(newDirPath.filename().c_str());
    file->Info.Mode |= mode;
    return 0;
}

int TFileSystem::MkDir(const char *path, mode_t mode) {
    const auto newDirPath = std::filesystem::path(path);
    auto parentDirRes = FindDir(newDirPath.parent_path());
    if(!parentDirRes) return PrintErrGetVal(parentDirRes);
    auto& parentDir = parentDirRes.value();
    const auto dir = parentDir->MakeFileObject<SDirectory>(newDirPath.filename().c_str());
    dir->Info.Mode |= S_IFDIR;
    return 0;
}

int TFileSystem::SymLink(const char *target_path, const char *link_path) {
    const auto targetRes = Find(target_path);
    if(!targetRes) return PrintErrGetVal(targetRes);
    const auto linkPath = std::filesystem::path(link_path);
    auto parentDirRes = FindDir(linkPath.parent_path());
    if(!parentDirRes) return PrintErrGetVal(parentDirRes);
    auto& parentDir = parentDirRes.value();
    const auto linkName = linkPath.filename().c_str();
    parentDir->MakeFileObject<SLink>(linkName, TraverseToRoot(targetRes.value()));
    return 0;
}

int TFileSystem::ChMod(const char *path, mode_t mode, struct fuse_file_info *fi) {
    const auto var = Find(path);
    if(!var) return PrintErrGetVal(var);
    std::visit([mode](const auto& p) { p->Info.Mode |= mode; }, var.value());
    return 0;
}

int TFileSystem::Read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    auto fileRes = FindFile(path);
    if(!fileRes) return PrintErrGetVal(fileRes);
    const auto& file = fileRes.value();
    memcpy(buffer, file->Content.data() + offset, size);
    return static_cast<int>(file->Content.size() - offset);
}

int TFileSystem::Write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info) {
    auto fileRes = FindFile(path);
    if(!fileRes) return PrintErrGetVal(fileRes);
    const auto& file = fileRes.value();
    file->Content.reserve(size);
    for(auto i = 0; i < offset; ++i) {
        file->Content.emplace_back(static_cast<std::byte>(buffer[i + offset]));
    }
    return 0;
}

int TFileSystem::ReadDir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    for(const auto& p : {s_sCurrentPath, s_sParentPath}) {
        filler(buffer, p.data(), NULL, 0, static_cast<fuse_fill_dir_flags>(flags));
    }
    const auto dirPath = std::filesystem::path(path);
    const auto& varRes = Find(path);
    if(!varRes) return PrintErrGetVal(varRes);
    const auto& var = varRes.value();

    return std::visit(SOverloadVariant {
        [buffer, filler, flags](const TStDirectory& dir) {
            FillerDirectory(dir, buffer, filler, flags);
            return 0;
        },
        [buffer, filler, flags](const TStLink& link) {
            const auto dirRes = FindDir(link->LinkTo);
            if(!dirRes) return PrintErrGetVal(dirRes);
            FillerDirectory(dirRes.value(), buffer, filler, flags);
            return 0;
        },
        [dirPath](const TStFile& file) {
            const std::expected<int, TFileSystemVariantException> err =
                std::unexpected(TNotDirectoryException(dirPath.begin(), dirPath.end()));
            return PrintErrGetVal(err);
        }
    }, var);
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
        return std::unexpected(TNotDirectoryException(path.begin(), it));
    }

    return tempDir;
}

std::expected<TStDirectory, TFileSystemVariantException> TFileSystem::FindDir(const std::filesystem::path& path) {
    const auto result = Find(path);
    if(!result) return std::unexpected(result.error());
    if(const auto t = std::get_if<TStDirectory>(&result.value())) {
        return *t;
    }
    return std::unexpected(TNotDirectoryException(path.begin(), path.end()));
}

std::expected<TStLink, TFileSystemVariantException> TFileSystem::FindLink(const std::filesystem::path& path) {
    const auto result = Find(path);
    if(!result) return std::unexpected(result.error());
    if(const auto t = std::get_if<TStLink>(&result.value())) {
        return *t;
    }
    return std::unexpected(TNotDirectoryException(path.begin(), path.end()));
}

std::expected<TStFile, TFileSystemVariantException> TFileSystem::FindFile(const std::filesystem::path& path) {
    const auto result = Find(path);
    if(!result) return std::unexpected(result.error());
    if(const auto t = std::get_if<TStFile>(&result.value())) {
        return *t;
    }
    return std::unexpected(TNotDirectoryException(path.begin(), path.end()));
}

std::filesystem::path TFileSystem::TraverseToRoot(const TStFileVariant& var) {
    auto names = std::vector<std::string_view>{GetFileObjectName(var)};
    auto parentDir = GetFileObjectParentDir(var).lock();
    while(parentDir != nullptr) {
        names.emplace_back(GetFileObjectName(parentDir));
        parentDir = parentDir->ParentDir.lock();
    }
    auto path = std::filesystem::path();
    for(const auto& n : names) path.append(n);
    return path;
}

std::string_view TFileSystem::GetFileObjectName(const TStFileVariant& var) {
    return std::visit([](const auto& obj) { return std::string_view(obj->Info.Name); }, var);
}

TWDirectory TFileSystem::GetFileObjectParentDir(const TStFileVariant& var) {
    return std::visit([](const auto& obj) {return obj->ParentDir;}, var);
}

void TFileSystem::FillerDirectory(const TStDirectory& dir, void* buffer, fuse_fill_dir_t filler, fuse_readdir_flags flags) {
    for(const auto& var : dir->FileVariants) {
        const auto name = GetFileObjectName(var);
        filler(buffer, name.data(), NULL, 0, static_cast<fuse_fill_dir_flags>(flags));
    }
}
