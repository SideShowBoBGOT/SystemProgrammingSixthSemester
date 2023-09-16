#ifndef LAB5_SDIRECTORY_H
#define LAB5_SDIRECTORY_H

#include <vector>
#include <optional>

#include <gsl/gsl>

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SDirectory {
    SDirectory(const std::string& name, const TNullableDirectory& parentDir);

    SFileSystemInfo Info;
    TWeakDirectory ParentDir;
    std::vector<TStrongFileVariant> FileVariants;
};


#endif //LAB5_SDIRECTORY_H
