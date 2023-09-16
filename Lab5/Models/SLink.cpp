#include <fcntl.h>

#include "SLink.h"

SLink::SLink(const std::string& name, const std::string& linkTo, const TNullableDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, LinkTo{linkTo}, ParentDir{parentDir} {
    Info.Mode = S_IFLNK | 0777;
}
