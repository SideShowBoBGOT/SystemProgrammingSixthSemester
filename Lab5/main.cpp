#include <iostream>


#define FUSE_USE_VERSION 30

#include <fuse3/fuse.h>


fuse_operations FileSystemOperations = {};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &FileSystemOperations, nullptr);
}