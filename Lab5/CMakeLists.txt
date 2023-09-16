cmake_minimum_required(VERSION 3.26)
project(Lab5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -Wall)


add_executable(${PROJECT_NAME} main.cpp
        NNFileSystem.cpp
        NNFileSystem.h
        SFileSystemInfo.cpp
        SFileSystemInfo.h
        SDirectory.cpp
        SDirectory.h
        SLink.cpp
        SLink.h
        SFile.cpp
        SFile.h
        TCommonDeclarations.h
)

include(FetchContent)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.0.0"
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)

include(FindPkgConfig)
pkg_search_module(FUSE3 REQUIRED fuse3)
target_include_directories(${PROJECT_NAME} PUBLIC ${FUSE3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FUSE3_LIBRARIES})


