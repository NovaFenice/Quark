cmake_minimum_required(VERSION 3.24)
project(qk VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/../_cmake_deps" CACHE PATH "FetchContent Cache")

include(FetchContent)

# ---- libarchive ----

FetchContent_Declare(
    libarchive
    SYSTEM
    URL      https://github.com/libarchive/libarchive/releases/download/v3.7.2/libarchive-3.7.2.tar.xz
    URL_HASH SHA256=04357661e6717b6941682cde02ad741ae4819c67a260593dfb2431861b251acb
)

# ---- JSON nlohmann (Header-only) ----

FetchContent_Declare(
    json
    URL      https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
)

# ---- Libcurl ----
FetchContent_Declare(
    curl
    URL      https://github.com/curl/curl/releases/download/curl-8_11_1/curl-8.11.1.tar.xz
    URL_HASH SHA256=c7ca7db48b0909743eaef34250da02c19bc61d4f1dcedd6603f109409536ab56
)

# ---- libarchive set ----

set(ENABLE_TAR OFF CACHE BOOL "" FORCE)
set(ENABLE_CPIO OFF CACHE BOOL "" FORCE)
set(ENABLE_CAT OFF CACHE BOOL "" FORCE)
set(ENABLE_XATTR OFF CACHE BOOL "" FORCE)
set(ENABLE_ACL OFF CACHE BOOL "" FORCE)
set(ENABLE_TEST OFF CACHE BOOL "" FORCE)

# ---- libcurl set ----

set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)      
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)        
set(CURL_DISABLE_DICT ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_FILE ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_FTP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_GOPHER ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_IMAP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_POP3 ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_RTSP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_SMTP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_TELNET ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_TFTP ON CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)


set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
set(CURL_USE_SECTRANSP ON CACHE BOOL "" FORCE)
set(CURL_CA_FALLBACK ON CACHE BOOL "" FORCE)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "" FORCE)

if(WIN32)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
    set(CURL_USE_SECTRANSP OFF CACHE BOOL "" FORCE)
else()
    set(CURL_USE_SCHANNEL OFF CACHE BOOL "" FORCE)
    set(CURL_USE_SECTRANSP ON CACHE BOOL "" FORCE)
endif()

set(CURL_CA_FALLBACK ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libarchive json curl)

add_executable(qk
    src/main.cpp
    src/EngineCompress.cpp
    src/utils.cpp
)

set_target_properties(qk PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_include_directories(qk PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${libarchive_SOURCE_DIR}/libarchive
)

target_link_libraries(qk PRIVATE 
    archive_static
    nlohmann_json::nlohmann_json
    libcurl
)

target_compile_definitions(qk PRIVATE LIBARCHIVE_STATIC)

if(WIN32)
    target_link_libraries(qk PRIVATE Bcrypt Rpcrt4 Crypt32 Ws2_32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(qk PRIVATE $<$<CONFIG:Release>:-O3>)
elseif(MSVC)
    target_compile_options(qk PRIVATE $<$<CONFIG:Release>:/Ox> $<$<CONFIG:Release>:/Ot>)
endif()

if(WIN32)
    add_custom_command(TARGET qk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:libcurl>
            $<TARGET_FILE_DIR:qk>
    )
    add_custom_command(TARGET qk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:archive>
            $<TARGET_FILE_DIR:qk>
    )
endif()