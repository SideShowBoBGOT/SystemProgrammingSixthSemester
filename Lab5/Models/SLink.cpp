#include <fcntl.h>

#include "SLink.h"

SLink::SLink(const std::string& name, const TWeakFileVariant &fileVariant, const TNullableDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, FileVariant{fileVariant}, ParentDir{parentDir} {
    Info.Mode = S_IFLNK | 0777;
}
