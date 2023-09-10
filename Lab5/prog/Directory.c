#include "Directory.h"

char *DirectoryGiveStr(const char *str)
{
    //  Function to allocating and returning string
    //  arguments:
    //          1) str      const char*     some string
    //  result:
    //          new_str      char*          some allocated string

    char *new_str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    new_str[0] = '\0';
    strcat(new_str, str);
    return new_str;
}
struct Directory *DirectoryInit(struct Directory *parent_dir, const char *name)
{
    //  Function to initializing pointer to instance of struct Directory
    //  arguments:
    //          1) parent_dir   struct Directory*   parent directory
    //          2) name         const char*         name of the future directory
    //  result:
    //          new_dir         struct Directory*      new directory

    struct Directory *new_dir = (struct Directory *)malloc(sizeof(struct Directory));
    new_dir->parent_dir = parent_dir;
    new_dir->self_dir = new_dir;
    new_dir->name = DirectoryGiveStr(name);
    new_dir->uid = getuid();
    new_dir->gid = getgid();
    new_dir->atime = time(NULL);
    new_dir->mtime = time(NULL);
    new_dir->n_link = 2;
    new_dir->files_num = 0;
    new_dir->mode = S_IFDIR | 0777;
    new_dir->sub_dirs = NULL;
    new_dir->files = NULL;
    new_dir->links = NULL;

    return new_dir;
}
struct File *DirectoryFileInit(struct Directory *parent_dir, const char *name)
{
    //  Function to initializing pointer to instance of struct File
    //  arguments:
    //          1) parent_dir   struct File*        parent directory
    //          2) name         const char*         name of the future file
    //  result:
    //          new_file        struct File*        new file

    struct File *new_file = (struct File *)malloc(sizeof(struct File));
    new_file->parent_dir = parent_dir;
    new_file->name = DirectoryGiveStr(name);
    new_file->uid = getuid();
    new_file->gid = getgid();
    new_file->atime = time(NULL);
    new_file->mtime = time(NULL);
    new_file->n_link = 1;
    new_file->content = DirectoryGiveStr("");
    new_file->size = 0;
    new_file->mode = S_IFREG | 0777;

    return new_file;
}
struct Link *DirectoryLinkInit(struct Directory *parent_dir, const char *name, const char *link_to)
{
    //  Function to initializing pointer to instance of struct Link
    //  arguments:
    //          1) parent_dir   struct link*        parent directory
    //          2) name         const char*         name of the future link
    //  result:
    //          new_link        struct link*        new link

    struct Link *new_link = (struct Link *)malloc(sizeof(struct Link));
    new_link->parent_dir = parent_dir;
    new_link->name = DirectoryGiveStr(name);
    new_link->uid = getuid();
    new_link->gid = getgid();
    new_link->atime = time(NULL);
    new_link->mtime = time(NULL);
    new_link->n_link = 1;
    new_link->mode = S_IFLNK | 0777;
    new_link->link_to = DirectoryGiveStr(link_to);
    new_link->size = strlen(new_link->link_to) - 1;

    return new_link;
}
struct Directory *DirectoryFindDir(const char *path)
{
    //  Function to find pointer to instance of struct Directory
    //  by ints path
    //  arguments:
    //          1) path     const char*             full path to the certain directory
    //  result:
    //          dir         struct Directory*       found directory
    //  !!!NOTE!!!                                                      !!!NOTE!!!
    //  !!!NOTE!!!          If path does not exist, dir = NULL          !!!NOTE!!!
    //  !!!NOTE!!!                                                      !!!NOTE!!!

    // get names of directories separately
    int dir_names_num;
    char **dir_names = DirectoryParsePath(path, &dir_names_num);
    // initial directory would be the root directory
    struct Directory *dir = root_dir;
    // find the directory
    // n_link number of sub_dirs + 2, where 2 are pointers
    // to itself and parent_dir accordingly
    int is_right_path = 0;
    for (int i = 0; i < dir_names_num; i++)
    {
        is_right_path = 0;
        for (int j = 0; j < dir->n_link - 2; j++)
        {
            if (strcmp(dir->sub_dirs[j]->name, dir_names[i]) == 0)
            {
                is_right_path = 1;
                dir = dir->sub_dirs[j];
                break;
            }
        }
        if (!is_right_path)
        {
            dir = NULL;
        }
    }
    // free memory allocated for dir_names
    for (int i = 0; i < dir_names_num; i++)
    {
        free(dir_names[i]);
    }
    if (dir_names_num)
    {
        free(dir_names);
    }
    return dir;
};
struct File *DirectoryFindFile(const char *path)
{
    //  Function to find pointer to instance of struct File
    //  by ints path
    //  arguments:
    //          1) path     const char*             full path to the certain file
    //  result:
    //          file         struct File*      found file
    //  !!!NOTE!!!                                                      !!!NOTE!!!
    //  !!!NOTE!!!          If path does not exist, file = NULL         !!!NOTE!!!
    //  !!!NOTE!!!                                                      !!!NOTE!!!

    // Get names of directories separately
    int dir_names_num;
    char **dir_names = DirectoryParsePath(path, &dir_names_num);
    // Find parent directory
    char *parent_path = DirectoryReParsePath(dir_names, dir_names_num - 1);
    struct Directory *parent_dir = DirectoryFindDir(parent_path);
    struct File *file = NULL;

    //  Find file in loop
    for (int i = 0; i < parent_dir->files_num && dir_names != NULL; i++)
    {

        if (strcmp(parent_dir->files[i]->name, dir_names[dir_names_num - 1]) == 0)
        {
            file = parent_dir->files[i];
            break;
        }
    }

    // Free memory allocated for dir_names
    for (int i = 0; i < dir_names_num; i++)
    {
        free(dir_names[i]);
    }
    if (dir_names_num)
    {
        free(dir_names);
    }

    return file;
}
struct Link *DirectoryFindLink(const char *path)
{
    //  Function to find pointer to instance of struct Link
    //  by ints path
    //  arguments:
    //          1) path     const char*             full path to the certain link
    //  result:
    //          link         struct Link*      found link
    //  !!!NOTE!!!                                                      !!!NOTE!!!
    //  !!!NOTE!!!          If path does not exist, link = NULL         !!!NOTE!!!
    //  !!!NOTE!!!                                                      !!!NOTE!!!

    // Get names of directories separately
    int dir_names_num;
    char **dir_names = DirectoryParsePath(path, &dir_names_num);
    // Find parent directory
    char *parent_path = DirectoryReParsePath(dir_names, dir_names_num - 1);
    struct Directory *parent_dir = DirectoryFindDir(parent_path);
    struct Link *link = NULL;

    //  Find link in loop
    for (int i = 0; i < parent_dir->links_num && dir_names != NULL; i++)
    {

        if (strcmp(parent_dir->links[i]->name, dir_names[dir_names_num - 1]) == 0)
        {
            link = parent_dir->links[i];
            break;
        }
    }

    // Free memory allocated for dir_names
    for (int i = 0; i < dir_names_num; i++)
    {
        free(dir_names[i]);
    }
    if (dir_names_num)
    {
        free(dir_names);
    }

    return link;
}
char **DirectoryParsePath(const char *path, int *length)
{
    //  Function to parse path into separate names of directories
    //  arguments:
    //          1) path     const char*     full path to the certain directory
    //          2) length   int*            int pointer which holds an adress to a number of words in names
    //  result:
    //          names       char**          array of names of directories
    //  Example 1:
    //          input: "/"
    //          result: names = NULL, length = 0
    //  Example 2:
    //          input: "/bar/baz"
    //          result: names[0] = "bar", names[1] = "baz", length = 2

    // assign two copies of path
    char *path_c1 = path;
    char *path_c2 = path;
    length[0] = 0;
    int index = 0;
    // count how many '/' in the path
    while ((*path_c1) != '\0')
    {
        // if "/" is the path iteself then it is root directory and length would be 0
        if ((*path_c1) == '/' && strlen(path_c1) > 1)
        {
            (*length)++;
        }
        path_c1++;
    }
    char **names = NULL;
    if ((*length))
    {
        names = (char **)malloc(sizeof(char *) * (*length));
        while ((*path_c2) != '\0')
        {
            if ((*path_c2) != '/')
            {
                names[index] = (char *)malloc(sizeof(char) * 255);
                names[index][0] = '\0';
                while ((*path_c2) != '/' && (*path_c2) != '\0')
                {
                    char buf[2];
                    buf[0] = (*path_c2);
                    buf[1] = '\0';
                    strcat(names[index], buf);
                    path_c2++;
                }
                path_c2--;
                index++;
            }
            path_c2++;
        }
    }
    return names;
}
char *DirectoryReParsePath(const char *const *dir_names, int dir_names_num)
{
    //  Function to reparse separate names of directories to full path
    //  arguments:
    //          1) dir_names        const char*const*   array of the names of the directories
    //          2) dir_names_num    int                 length of the array of the names of the directories
    //  result:
    //          path                char*               path that consists of the elements of the array of the names of the directories
    //  Example 1:
    //          input:  dir_names[0] = "baz", dir_names[1] = "bar", dir_names_num = 2
    //          result: path = "/baz/bar"

    char *path = (char *)malloc(sizeof(char) * 255);
    path[0] = '/';
    path[1] = '\0';

    for (int i = 0; i < dir_names_num; i++)
    {
        if (i == 0)
        {
            strcat(path, dir_names[i]);
        }
        else
        {
            strcat(strcat(path, "/"), dir_names[i]);
        }
    }

    return path;
}
void DirectoryAddSubDir(struct Directory *parent_dir, struct Directory *new_sub_dir)
{
    //  Function to adding to the array of subdirectories of the parent directory
    //  the new subdirectory
    //  arguments:
    //          1) parent_dir           struct Directory*        parent directory
    //          2) new_sub_dir          struct Directory*        new subdirectory
    //  result:
    //          void

    // increment number of subdirectories
    parent_dir->n_link++;
    struct Directory **new_sub_dirs = (struct Directory **)malloc(sizeof(struct Directory *) * (parent_dir->n_link - 2));
    // allocate old array of subdirs into new one
    for (int i = 0; i < parent_dir->n_link - 2 - 1; i++)
    {
        new_sub_dirs[i] = parent_dir->sub_dirs[i];
    }
    // deallocate old array of subdirs if it existed before
    if (parent_dir->n_link - 2 - 1)
    {
        free(parent_dir->sub_dirs);
    }
    // add new subdirectory to array of subdirectories of parent directory
    new_sub_dirs[parent_dir->n_link - 2 - 1] = new_sub_dir;
    // assign new array of subdirectories to parent directory`s one
    parent_dir->sub_dirs = new_sub_dirs;
}
void DirectoryAddFile(struct Directory *parent_dir, struct File *new_file)
{
    //  Function to adding to the array of files of the parent directory
    //  the new file
    //  arguments:
    //          1) parent_dir           struct Directory*        parent directory
    //          2) new_file             struct File*             new file
    //  result:
    //          void

    // increment number of files

    parent_dir->files_num++;
    struct File **new_files = (struct File **)malloc(sizeof(struct File *) * parent_dir->files_num);
    // allocate old array of files into new one
    for (int i = 0; i < parent_dir->files_num - 1; i++)
    {
        new_files[i] = parent_dir->files[i];
    }

    // deallocate old array of files if it existed before
    if (parent_dir->files_num - 1)
    {
        free(parent_dir->files);
    }

    // add new file to array of files of parent directory
    new_files[parent_dir->files_num - 1] = new_file;

    // assign new array of files to parent directory`s one
    parent_dir->files = new_files;
}
void DirectoryAddLink(struct Directory *parent_dir, struct Link *new_link)
{
    //  Function to adding to the array of links of the parent directory
    //  the new link
    //  arguments:
    //          1) parent_dir           struct Directory*        parent directory
    //          2) new_link             struct Link*             new link
    //  result:
    //          void

    // increment number of links

    parent_dir->links_num++;
    struct Link **new_links = (struct link **)malloc(sizeof(struct Link *) * parent_dir->links_num);
    // allocate old array of links into new one
    for (int i = 0; i < parent_dir->links_num - 1; i++)
    {
        new_links[i] = parent_dir->links[i];
    }

    // deallocate old array of links if it existed before
    if (parent_dir->links_num - 1)
    {
        free(parent_dir->links);
    }

    // add new link to array of links of parent directory
    new_links[parent_dir->links_num - 1] = new_link;

    // assign new array of links to parent directory`s one
    parent_dir->links = new_links;
}
