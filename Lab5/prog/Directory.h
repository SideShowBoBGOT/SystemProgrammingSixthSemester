// author: choleraplague aka sideshowbobgot
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

struct Directory
{
    //  !!!NOTE!!!    To see what these attributes mean     !!!NOTE!!!
    //  !!!NOTE!!!    see the next link:                    !!!NOTE!!!
    //  https://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html

    //  Specifies the name of the file.
    char *name;
    //  The user ID of the file’s owner.
    uid_t uid;
    //  The group ID of the file.
    gid_t gid;
    //  This is the last access time for the file.
    time_t atime;
    //  This is the time of the last modification to the contents of the file.
    time_t mtime;
    //  Specifies the mode of the file.
    //  This includes file type informationand the file permission bits
    mode_t mode;
    //  The number of hard links to the file.
    //  This count keeps track of how many directories have entries for this file.
    nlink_t n_link;
    //  Specifies the amount of files the directory has
    int files_num;
    //  Specifies the amount of links the directory has
    int links_num;
    //  Pointer to parent directory
    struct Directory *parent_dir;
    //  Pointer to itself
    struct Directory *self_dir;
    //  Array of subdirectories
    struct Directory **sub_dirs;
    //  Array of files in the directory
    struct File **files;
    //  Array of links in the directory
    struct Link **links;
};
struct File
{
    //  !!!NOTE!!!    To see what these attributes mean     !!!NOTE!!!
    //  !!!NOTE!!!    see the next link:                    !!!NOTE!!!
    //  https://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html

    //  Specifies the name of the file.
    char *name;
    //  The user ID of the file’s owner.
    uid_t uid;
    //  The group ID of the file.
    gid_t gid;
    //  This is the last access time for the file.
    time_t atime;
    //  This is the time of the last modification to the contents of the file.
    time_t mtime;
    //  Specifies the mode of the file.
    //  This includes file type informationand the file permission bits
    mode_t mode;
    //  The number of hard links to the file.
    //  This count keeps track of how many directories have entries for this file.
    nlink_t n_link;
    //  Specifies the size of the file
    off_t size;
    //  Specifies the content of the file
    char *content;
    //  Pointer to parent directory
    struct Directory *parent_dir;
};
struct Link
{
    //  !!!NOTE!!!    To see what these attributes mean     !!!NOTE!!!
    //  !!!NOTE!!!    see the next link:                    !!!NOTE!!!
    //  https://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html

    //  Specifies the name of the link.
    char *name;
    //  The user ID of the link’s owner.
    uid_t uid;
    //  The group ID of the link.
    gid_t gid;
    //  This is the last access time for the link.
    time_t atime;
    //  This is the time of the last modification to the contents of the link.
    time_t mtime;
    //  Specifies the mode of the link.
    //  This includes link type informationand the link permission bits
    mode_t mode;
    //  The number of hard links to the link.
    //  This count keeps track of how many directories have entries for this link.
    nlink_t n_link;
    //  Specifies the size of the link
    off_t size;
    //  Specifies the path of the file of directory the link refers to
    char *link_to;
    //  Pointer to parent directory
    struct Directory *parent_dir;
};
struct Directory *root_dir;
char *DirectoryGiveStr(const char *str);
struct Directory *DirectoryInit(struct Directory *parent_dir, const char *name);
struct File *DirectoryFileInit(struct Directory *parent_dir, const char *name);
struct Link *DirectoryLinkInit(struct Directory *parent_dir, const char *name, const char *link_to);
struct Directory *DirectoryFindDir(const char *path);
struct File *DirectoryFindFile(const char *path);
struct Link *DirectoryFindLink(const char *path);
void DirectoryAddSubDir(struct Directory *parent_dir, struct Directory *new_sub_dir);
void DirectoryAddFile(struct Directory *parent_dir, struct File *new_file);
void DirectoryAddLink(struct Directory *parent_dir, struct Link *new_link);
char **DirectoryParsePath(const char *path, int *length);
char *DirectoryReParsePath(const char *const *dir_names, int dir_names_num);

#endif DIRECTORY_H_INCLUDED