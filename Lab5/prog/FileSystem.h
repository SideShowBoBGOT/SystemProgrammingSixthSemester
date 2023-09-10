// author: choleraplague aka sideshowbobgot
#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

#include "Directory.h"

#ifndef FILE_SYSTEM_H_INCLUDED
#define FILE_SYSTEM_H_INCLUDED

int FileSystemGETATTR(const char *path, struct stat *st);
int FileSystemMKDIR(const char *path, mode_t mode);
int FileSystemREADDIR(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
int FileSystemMKNOD(const char *path, mode_t mode, dev_t rdev);
int FileSystemREAD(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi);
int FileSystemWRITE(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info);
int FileSystemCHMOD(const char *path, mode_t mode);
int FileSystemSYMLINK(const char *target_path, const char *link_path);
int FileSystemREADLINK(const char *path, char *buffer, size_t size);

#endif FILE_SYSTEM_H_INCLUDED