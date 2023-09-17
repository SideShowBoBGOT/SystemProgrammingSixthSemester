#ifndef LAB5_SDIRECTORY_H
#define LAB5_SDIRECTORY_H

#include <vector>
#include <optional>

#include <gsl/gsl>

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SDirectory : std::enable_shared_from_this<SDirectory> {
    SDirectory(const std::string& name, const TOptStDirectory& parentDir);

    SFileSystemInfo Info;
    TWDirectory ParentDir;
    std::vector<TStFileVariant> FileVariants;
};


#endif //LAB5_SDIRECTORY_H
