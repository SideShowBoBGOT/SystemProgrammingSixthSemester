#ifndef LAB5_TFILESYSTEM_H
#define LAB5_TFILESYSTEM_H

#define FUSE_USE_VERSION 30

#include <vector>
#include <expected>

#include <fuse3/fuse.h>

#include "TCommonDeclarations.h"

class TNotDirectoryException;

class TFileSystem {
    public:
    static int GetAttr(const char *path, struct stat *st);
    static int MkDir(const char *path, mode_t mode);
    static int ReadDir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
    static int MkNod(const char *path, mode_t mode, dev_t rdev);
    static int Read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi);
    static int Write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info);
    static int ChMod(const char *path, mode_t mode);
    static int SymLink(const char *target_path, const char *link_path);
    static int ReadLink(const char *path, char *buffer, size_t size);

    private:
    [[nodiscard]] static std::expected<TStrongFileVariant, TNotDirectoryException> Find(const std::string_view& path);
    static std::vector<std::string_view> ParsePath(const std::string_view& path);

    private:
    static const TStrongDirectory s_pRootDir;
};


#endif //LAB5_TFILESYSTEM_H
