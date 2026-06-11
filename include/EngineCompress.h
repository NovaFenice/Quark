#ifndef ENGINE_COMPRESS_H
#define ENGINE_COMPRESS_H

#include "conversions.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <variant>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

struct ComandCompress {
    FORMAT::ARCHIVE ext;
    std::filesystem::path path_in;
    std::filesystem::path path_out;
    bool encryption = false;
    std::string encryption_pass;
};

struct CompressionSuccess {};
struct FormatError        { std::string message; };
struct IOError            { std::string message; };

using ArchiveResult = std::variant<CompressionSuccess, FormatError, IOError>;

static int copy_data_stream(struct archive *ar, struct archive* aw);
static bool write_file_to_archive(struct archive* a, const fs::path& file_path, const fs::path& entry_path);

ArchiveResult compress(const ComandCompress& cmd);
bool decompress(const ComandCompress& cmd);

#define SET_TAR_FORMAT(archive_ptr) \
    archive_write_set_format_pax_restricted(archive_ptr); \
    break
#endif