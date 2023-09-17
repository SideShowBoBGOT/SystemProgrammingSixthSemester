#include <fcntl.h>

#include "SLink.h"
#include "../Helpers/THelpers.h"

SLink::SLink(const std::string& name, const TStFileVariant& linkTo, const TStDirectory& parentDir)
    : Info{SFileSystemInfo(name)}, LinkTo{DowngradeVariant(linkTo)}, ParentDir{parentDir.get()} {
    Info.Mode = S_IFLNK | 0777;
}
