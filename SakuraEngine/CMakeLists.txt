cmake_minimum_required(VERSION 3.11)

project(Sakura VERSION 0.1.0)
execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/SPA/anyToheader.py meta .json ${CMAKE_SOURCE_DIR}/SakuraEngine 
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SPA)
execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/SPA/ModuleCollector.py meta  ${CMAKE_SOURCE_DIR}/SakuraEngine 
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SPA)

add_definitions(-D "SAKURA_CONSOLE_INOUT")
add_definitions(-D "SAKURA_DEBUG_EDITOR")
add_definitions(-D "SAKURA_DEBUG_GAME")

ADD_DEFINITIONS(-DMACRO)
if(UNIX)
    add_definitions(-D "SAKURA_TARGET_PLATFORM_UNIX")
    if(APPLE)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_OSX")
        set(OS_MACOS 1)
    elseif(${CMAKE_SYSTEM_NAME}  MATCHES ANDROID)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_ANDROID")
        set(OS_ANDROID 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES Emscripten)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_EMSCRIPTEN")
        set(WA 1)   #Web
        set(OS_WEBASSEMBLY 1)
    else(APPLE)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_LINUX")
        set(OS_LINUX 1)
    endif(APPLE)
elseif(WIN32)
    add_definitions(-D "SAKURA_TARGET_PLATFORM_WIN32")
    message(STATUS "Platform WIN32")
    set(OS_WINDOWS 1)
endif(UNIX)


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

enable_testing()

include_directories(
    ${PROJECT_SOURCE_DIR}
)
include_directories(
    ../Extern/include
)
include_directories(
    ../
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(Core)
add_subdirectory(StaticBuilds)
add_subdirectory(Modules)
add_subdirectory(UnitTests)
add_subdirectory(Editor)
add_subdirectory(Framework)
add_subdirectory(Applications)

include(CPack)