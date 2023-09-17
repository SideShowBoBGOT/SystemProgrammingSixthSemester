#include <format>

#include "TPathException.h"

#define DECLARE_PATH_EXCEPTION(name)\
    const char* T##name##Exception::what() const noexcept {\
        return m_sMessage.c_str();\
    }

    DECLARE_PATH_EXCEPTION(NotDirectory);
    DECLARE_PATH_EXCEPTION(FileObjectNotExist);

#undef DECLARE_PATH_EXCEPTION