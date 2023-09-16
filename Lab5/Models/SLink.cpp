#include "SLink.h"

SLink::SLink(const SFileSystemInfo &info, const TWeakFileVariant &fileVariant, const TNullableDirectory& parentDir)
    : Info{info}, FileVariant{fileVariant}, ParentDir{parentDir} {}
