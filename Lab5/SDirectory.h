#ifndef LAB5_SDIRECTORY_H
#define LAB5_SDIRECTORY_H

#include <vector>

#include <gsl/gsl>

#include "TFileVariant.h"
#include "SFileSystemInfo.h"

class SLink;
class TFile;

class SDirectory {

    SFileSystemInfo Info;
    not_null_shared_ptr<SDirectory> ParentDir;
    std::vector<PDirectory> SubDirs;
    std::vector<PLink> Links;
    std::vector<PFile> Files;
};


#endif //LAB5_SDIRECTORY_H
