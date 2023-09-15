#include <iostream>

#include "NNFileSystem.h"

fuse_operations FileSystemOperations = {};

int main(int argc, char *argv[]) {
    return fuse_main_compat2(argc, argv, &FileSystemOperations);
}
