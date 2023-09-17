#include <unistd.h>
#include "TPathException.h"

#define DECLARE_PATH_EXCEPTION(name, errorCode)\
    const char* T##name##Exception::what() const noexcept {\
        return m_sMessage.c_str();\
    }                               \
    int T##name##Exception::Value() const noexcept {                      \
        return errorCode;                                \
    }
    DECLARE_PATH_EXCEPTION(NotDirectory, ENOTDIR);
    DECLARE_PATH_EXCEPTION(NotLink, ENOLINK);
    DECLARE_PATH_EXCEPTION(NotFile, ENOENT);
    DECLARE_PATH_EXCEPTION(FileObjectNotExist, ENOENT);

#undef DECLARE_PATH_EXCEPTION