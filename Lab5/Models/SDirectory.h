#ifndef LAB5_SDIRECTORY_H
#define LAB5_SDIRECTORY_H

#include <vector>
#include <optional>

#include <gsl/gsl>

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"
#include "../Helpers/THelpers.h"

struct SDirectory : std::enable_shared_from_this<SDirectory> {
    SDirectory(const std::string& name, const TOptStDirectory& parentDir);
    template<typename T, typename... Args>
    gsl::not_null<std::shared_ptr<T>> MakeFileObject(Args... args) {
        const auto fileObject = MakeNotNull<T>(args..., shared_from_this());
        FileVariants.emplace_back(fileObject);
        return fileObject;
    }
    SFileSystemInfo Info;
    TWDirectory ParentDir;
    std::vector<TStFileVariant> FileVariants;
};


#endif //LAB5_SDIRECTORY_H
