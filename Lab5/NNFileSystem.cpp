#include "NNFileSystem.h"

int NNFileSystem::GetAttr(const char *path, struct stat *st) {
    return 0;
}

int NNFileSystem::MkDir(const char *path, mode_t mode) {
    return 0;
}

int
NNFileSystem::ReadDir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    return 0;
}

int NNFileSystem::MkNod(const char *path, mode_t mode, dev_t rdev) {
    return 0;
}

int NNFileSystem::Read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    return 0;
}

int NNFileSystem::Write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info) {
    return 0;
}

int NNFileSystem::ChMod(const char *path, mode_t mode) {
    return 0;
}

int NNFileSystem::SymLink(const char *target_path, const char *link_path) {
    return 0;
}

int NNFileSystem::ReadLink(const char *path, char *buffer, size_t size) {
    return 0;
}
