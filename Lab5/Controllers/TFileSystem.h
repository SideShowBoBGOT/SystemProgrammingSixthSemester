#ifndef LAB5_TFILESYSTEM_H
#define LAB5_TFILESYSTEM_H

#define FUSE_USE_VERSION 30

#include <vector>
#include <expected>
#include <filesystem>

#include <fuse3/fuse.h>

#include "../Models/TCommonDeclarations.h"

class TFileSystem {
    public:
    static int GetAttr(const char *path, struct stat *st, struct fuse_file_info *fi);
    static int ReadLink(const char *path, char *buffer, size_t size);
    static int MkNod(const char *path, mode_t mode, dev_t rdev);
    static int MkDir(const char *path, mode_t mode);
    static int SymLink(const char *target_path, const char *link_path);
    static int ChMod(const char *path, mode_t mode, struct fuse_file_info *fi);
    static int Read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi);
    static int Write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info);
    static int ReadDir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags);

    private:
    [[nodiscard]] static auto Find(const std::filesystem::path& path)
        -> std::expected<TStFileVariant, TFileSystemVariantException>;
    [[nodiscard]] static auto FindDir(const std::filesystem::path& path)
    -> std::expected<TStDirectory, TFileSystemVariantException>;
    [[nodiscard]] static auto FindLink(const std::filesystem::path& path)
    -> std::expected<TStLink, TFileSystemVariantException>;
    [[nodiscard]] static auto FindFile(const std::filesystem::path& path)
    -> std::expected<TStFile, TFileSystemVariantException>;
    static std::filesystem::path TraverseToRoot(const TStFileVariant& var);
    static std::string_view GetFileObjectName(const TStFileVariant& var);
    static TWDirectory GetFileObjectParentDir(const TStFileVariant& var);
    static void FillerBuffer(const std::string_view& name , void *buffer, fuse_fill_dir_t filler);
    static void FillerDirectory(const TStDirectory& dir, void *buffer, fuse_fill_dir_t filler);
    static void ModifyAttr(const auto& fileObject, struct stat *st);

    private:
    static const TStDirectory s_pRootDir;
};


#endif //LAB5_TFILESYSTEM_H
