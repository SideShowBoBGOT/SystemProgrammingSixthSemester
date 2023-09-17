#ifndef LAB5_TPATHEXCEPTION_H
#define LAB5_TPATHEXCEPTION_H

#include <exception>
#include <string>
#include <filesystem>

#define DECLARE_PATH_EXCEPTION(name, message)\
    class T##name##Exception : public std::exception {\
        public:\
        template<typename PathIterator>\
        T##name##Exception(const PathIterator& begin, const PathIterator& end) {\
            auto path = std::filesystem::path();\
            for(auto it = begin; it != end; ++it) path.append(it->c_str());\
            m_sMessage = std::format(message, std::string_view(path.c_str()));\
        }\
        virtual const char* what() const noexcept override;\
                                    \
        protected:\
        std::string m_sMessage;\
    };

    DECLARE_PATH_EXCEPTION(NotDirectory, "File object is not directory: {}");
    DECLARE_PATH_EXCEPTION(FileObjectNotExist, "File does not exist: {}");

#undef DECLARE_PATH_EXCEPTION

#endif //LAB5_TPATHEXCEPTION_H
