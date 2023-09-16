#include <format>

#include "TPathException.h"

#define DECLARE_PATH_EXCEPTION(name, message) \
    static constexpr std::string_view s_s##name##Message = message;\
    T##name##Exception::T##name##Exception(const std::string_view& path) {\
        m_sMessage = std::format(s_s##name##Message, path);\
    }\
    const char* T##name##Exception::what() const noexcept {\
        return m_sMessage.c_str();\
    }

    DECLARE_PATH_EXCEPTION(NotDirectory, "File object is not directory: {}");
    DECLARE_PATH_EXCEPTION(FileObjectNotExist, "File does not exist: {}");

#undef DECLARE_PATH_EXCEPTION