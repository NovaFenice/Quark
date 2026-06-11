cmake_minimum_required(VERSION 3.24)
project(qk VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/../_cmake_deps" CACHE PATH "FetchContent Cache")

include(FetchContent)

FetchContent_Declare(
    libarchive
    URL      https://github.com/libarchive/libarchive/releases/download/v3.7.2/libarchive-3.7.2.tar.xz
    URL_HASH SHA256=04357661e6717b6941682cde02ad741ae4819c67a260593dfb2431861b251acb
)

set(ENABLE_TAR OFF CACHE BOOL "" FORCE)
set(ENABLE_CPIO OFF CACHE BOOL "" FORCE)
set(ENABLE_CAT OFF CACHE BOOL "" FORCE)
set(ENABLE_XATTR OFF CACHE BOOL "" FORCE)
set(ENABLE_ACL OFF CACHE BOOL "" FORCE)
set(ENABLE_TEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libarchive)

add_executable(qk
    src/main.cpp
    src/EngineCompress.cpp
)

target_include_directories(qk PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${libarchive_SOURCE_DIR}/libarchive
)

target_link_libraries(qk PRIVATE archive_static)

target_compile_definitions(qk PRIVATE LIBARCHIVE_STATIC)

if(WIN32)
    target_link_libraries(qk PRIVATE Bcrypt Rpcrt4)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(qk PRIVATE $<$<CONFIG:Release>:-O3>)
elseif(MSVC)
    target_compile_options(qk PRIVATE $<$<CONFIG:Release>:/Ox> $<$<CONFIG:Release>:/Ot>)
endif()