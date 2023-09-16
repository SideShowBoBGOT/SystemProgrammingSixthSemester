#include "SLink.h"

SLink::SLink(const SFileSystemInfo &info, const TWeakFileVariant &fileVariant, const not_null_shared_ptr<SDirectory> &parentDir)
    : Info{info}, FileVariant{fileVariant}, ParentDir{parentDir.get()} {}
