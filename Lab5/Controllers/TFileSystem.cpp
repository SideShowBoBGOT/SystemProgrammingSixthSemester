#include <ranges>
#include <algorithm>

#include "TFileSystem.h"
#include "../Models/SDirectory.h"
#include "../Models/SLink.h"
#include "../Models/SFile.h"
#include "../Helpers/SOverloadVariant.h"
#include "../Models/Errors/TPathException.h"

const TStrongDirectory TFileSystem::s_pRootDir = std::make_shared<SDirectory>("", nullptr);

int TFileSystem::GetAttr(const char *path, struct stat *st) {

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

int TFileSystem::SymLink(const char *target_path, const char *link_path) {
    return 0;
}

int TFileSystem::ReadLink(const char *path, char *buffer, size_t size) {
    return 0;
}

std::expected<TStrongFileVariant, TFileSystemVariantException> TFileSystem::Find(const std::string_view& path) {
    const auto names = PathToNames(path);
    if(names.empty()) return s_pRootDir;

    auto tempDir = s_pRootDir;

    for(const auto& [index, n] : std::views::enumerate(names)) {
        const auto it = std::find_if(tempDir->FileVariants.begin(), tempDir->FileVariants.end(),
            [&n](const TStrongFileVariant& var) {
                return std::visit(SOverloadVariant {
                    [&n](const TStrongFile& file) { return file->Info.Name == n; },
                    [&n](const TStrongLink& link) { return link->Info.Name == n; },
                    [&n](const TStrongDirectory& dir) { return dir->Info.Name == n; }
                }, var);
            }
        );
        if(it==tempDir->FileVariants.end()) {
            return std::unexpected(TFileObjectNotExistException(NamesToPath(names, index)));
        }
        if(index == names.size() - 1) {
            return *it;
        }
        if(const auto t = std::get_if<TStrongDirectory>(&*it)) {
            tempDir = *t;
            continue;
        }
        return std::unexpected(TNotDirectoryException(NamesToPath(names, index)));
    }

    return std::unexpected(TFileObjectNotExistException(NamesToPath(names, static_cast<long>(names.size()) - 1)));
}

std::vector<std::string_view> TFileSystem::PathToNames(const std::string_view& path) {
    auto names = std::vector<std::string_view>();
    auto temp = path;
    while(!temp.empty()) {
        const auto firstSlashPos = temp.find('/');
        if(firstSlashPos == std::string_view::npos) break;
        const auto secondSlashPos = temp.find('/', firstSlashPos);
        const auto n = temp.substr(firstSlashPos, secondSlashPos);
        if(n.empty()) break;
        names.push_back(n);
        temp = temp.substr(secondSlashPos);
    }
    return names;
}

std::string TFileSystem::NamesToPath(const std::vector<std::string_view>& names, long endPos) {
    auto path = std::string();
    for(const auto& el : std::views::take(names, endPos + 1)) {
        path += "/" + std::string(el.begin(), el.end());
    }
    return path;
}
