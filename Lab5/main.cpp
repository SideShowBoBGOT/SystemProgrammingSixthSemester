#include "Controllers/TFileSystem.h"
#include "Models/SDirectory.h"
fuse_operations FileSystemOperations = {};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &FileSystemOperations, nullptr);
}