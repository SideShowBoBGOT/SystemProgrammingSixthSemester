#ifndef LAB5_SDIRECTORY_H
#define LAB5_SDIRECTORY_H

#include <vector>
#include <optional>

#include <gsl/gsl>

#include "TCommonDeclarations.h"
#include "SFileSystemInfo.h"

struct SDirectory {
    SDirectory(const SFileSystemInfo& info, const TStrongDirectory& parentDir);

    SFileSystemInfo Info;
    TWeakDirectory ParentDir;
    std::vector<TStrongFileVariant> FileVariants;

    private:
    template<typename FileObjectCont>
    FileObjectCont::iterator FindByName(FileObjectCont& objects, const std::string_view& name) {
        return std::find_if(objects.begin(), objects.end(), [&name](const auto& obj) { return obj->Info.Name==name; });
    }
};


#endif //LAB5_SDIRECTORY_H
