#ifndef LAB5_TPATHEXCEPTION_H
#define LAB5_TPATHEXCEPTION_H

#include <exception>
#include <string>

#define DECLARE_PATH_EXCEPTION(name)\
    class T##name##Exception : public std::exception {\
        public:\
        T##name##Exception()=default;\
        explicit T##name##Exception(const std::string_view& path);\
        virtual const char* what() const noexcept override;\
                                    \
        protected:\
        std::string m_sMessage;\
    };

    DECLARE_PATH_EXCEPTION(NotDirectory);
    DECLARE_PATH_EXCEPTION(FileObjectNotExist);

#undef DECLARE_PATH_EXCEPTION

#endif //LAB5_TPATHEXCEPTION_H
