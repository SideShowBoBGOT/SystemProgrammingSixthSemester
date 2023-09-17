#include <fcntl.h>

#include "SLink.h"
#include "../Helpers/SOverloadVariant.h"

SLink::SLink(const std::string& name, const TStFileVariant& linkTo, const TOptStDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, LinkTo{DowngradeVariant(linkTo)}, ParentDir{parentDir} {
    Info.Mode = S_IFLNK | 0777;
}
