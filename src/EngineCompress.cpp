#include "EngineCompress.h"

static int copy_data_stream(struct archive *ar, struct archive* aw) {
    int r;
    const void* buff;
    size_t size;
    la_int64_t offset;

    while (true) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF) return ARCHIVE_OK;
        if (r < ARCHIVE_OK) return r;

        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) return r;
    }
}

using TimePoint = std::chrono::steady_clock::time_point;

static std::string format_time(uint64_t total_seconds) {
    int d = total_seconds / 86400;
    int h = (total_seconds % 86400) / 3600;
    int m = (total_seconds % 3600) / 60;
    int s = total_seconds % 60;

    std::ostringstream oss;
    oss << std::setfill('0');
    if (d > 0) {
        oss << std::setw(2) << d << ":" << std::setw(2) << h << ":" << std::setw(2) << m << ":" << std::setw(2) << s;
    } else {
        oss << std::setw(2) << h << ":" << std::setw(2) << m << ":" << std::setw(2) << s;
    }
    return oss.str();
}

static void print_progress(uint64_t current, uint64_t total, TimePoint start_time, bool force = false) {
    if (total == 0) return;
    
    static TimePoint last_update = TimePoint{};
    static TimePoint last_estimate_time = TimePoint{};
    static uint64_t cached_estimated = 0;

    if (current == 0) {
        last_update = TimePoint{};
        last_estimate_time = TimePoint{};
        
        cached_estimated = total / (250 * 1024 * 1024);
        if (cached_estimated == 0) cached_estimated = 1;

        std::cout << "\x1b[?25l";
    }

    auto now = std::chrono::steady_clock::now();
    
    if (!force && std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update).count() < 200) {
        return;
    }
    last_update = now;

    int barWidth = 50;
    float progress = static_cast<float>(current) / total;
    if (progress > 1.0f) progress = 1.0f;
    int pos = static_cast<int>(barWidth * progress);

    uint64_t elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
    
    if (progress > 0.0f) {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_estimate_time).count() >= 3 || force) {
            cached_estimated = static_cast<uint64_t>(elapsed_seconds / progress);
            last_estimate_time = now;
        }
    }

    std::cout << "\r[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " % | " 
              << format_time(elapsed_seconds) << " / " << format_time(cached_estimated) 
              << "   " << std::flush;
              
    if (current >= total) {
        std::cout << "\x1b[?25h";
    }
}

static bool write_file_to_archive(struct archive* a, const fs::path& file_path, const fs::path& entry_path, uint64_t total_bytes, uint64_t& processed_bytes, TimePoint start_time) {
    constexpr size_t BUFFER_SIZE = 4 * 1024 * 1024;
    std::vector<char> buff(BUFFER_SIZE);
    
    std::ifstream file_in(file_path, std::ios::binary);
    if (!file_in.is_open()) return false;

    struct archive_entry* entry = archive_entry_new();
    archive_entry_set_pathname(entry, entry_path.string().c_str());
    archive_entry_set_size(entry, fs::file_size(file_path));
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);

    if (archive_write_header(a, entry) != ARCHIVE_OK) {
        archive_entry_free(entry);
        return false;
    }

    while (file_in.read(buff.data(), buff.size()) || file_in.gcount() > 0) {
        if (archive_write_data(a, buff.data(), file_in.gcount()) < 0) break;
        processed_bytes += file_in.gcount();
        print_progress(processed_bytes, total_bytes, start_time);
    }

    archive_entry_free(entry);
    return true;
}

ArchiveResult compress(const ComandCompress& cmd) {
    if (!fs::exists(cmd.path_in)) {
        return FormatError{"Input path does not exist."};
    }

    uint64_t total_bytes = 0;
    if (fs::is_directory(cmd.path_in)) {
        for (const auto& dir_entry : fs::recursive_directory_iterator(cmd.path_in)) {
            if (fs::is_regular_file(dir_entry.path())) {
                total_bytes += fs::file_size(dir_entry.path());
            }
        }
    } else if (fs::is_regular_file(cmd.path_in)) {
        total_bytes = fs::file_size(cmd.path_in);
    }

    std::cout << "Compressing: " << cmd.path_in.filename().string() << "...\n";

    struct archive* a = archive_write_new();

    switch (cmd.ext) {
        case FORMAT::ARCHIVE::ZIP:
        case FORMAT::ARCHIVE::JAR:
            archive_write_set_format_zip(a);
            break;
        
        case FORMAT::ARCHIVE::_7Z:
            archive_write_set_format_7zip(a);
            break;
        
        case FORMAT::ARCHIVE::CPIO:
            archive_write_set_format_cpio(a);
            break;

        case FORMAT::ARCHIVE::TAR:
            SET_TAR_FORMAT(a);

        case FORMAT::ARCHIVE::TGZ:
            archive_write_add_filter_gzip(a);
            SET_TAR_FORMAT(a);
        
        case FORMAT::ARCHIVE::TAR_XZ:
            archive_write_add_filter_xz(a);
            SET_TAR_FORMAT(a);

        case FORMAT::ARCHIVE::TAR_BZ:
        case FORMAT::ARCHIVE::TBZ2:
            archive_write_add_filter_bzip2(a);
            SET_TAR_FORMAT(a);

        case FORMAT::ARCHIVE::TAR_LZMA:
            archive_write_add_filter_lzma(a);
            SET_TAR_FORMAT(a);

        case FORMAT::ARCHIVE::TAR_Z:
            archive_write_add_filter_compress(a);
            SET_TAR_FORMAT(a);
        
        case FORMAT::ARCHIVE::TAR_LZO:
            if (archive_write_add_filter_lzop(a) != ARCHIVE_OK) {
                archive_write_free(a);
                return FormatError{"LZO compression is not supported"};
            }
            SET_TAR_FORMAT(a);

        case FORMAT::ARCHIVE::TAR_LZ:
            if (archive_write_add_filter_lzip(a) != ARCHIVE_OK) {
                archive_write_free(a);
                return FormatError{"LZ compression is not supported"};
            }
            SET_TAR_FORMAT(a);

        case FORMAT::ARCHIVE::TAR_7Z: {
            std::string zip7_cmd = "7z a -si -an -txz";
            
            if (cmd.encryption && !cmd.encryption_pass.empty())
                zip7_cmd += " -p" + cmd.encryption_pass + " -mhe=on";

            if (archive_write_add_filter_program(a, zip7_cmd.c_str()) != ARCHIVE_OK) {
                archive_write_free(a);
                return FormatError{"7z compression is not supported. Install '7z' or 'p7zip' and ensure it's in your system PATH."};
            }
            SET_TAR_FORMAT(a);
        } break;

        case FORMAT::ARCHIVE::RAR:
            if (archive_write_add_filter_program(a, "rar a -si") != ARCHIVE_OK) {
                archive_write_free(a);
                return FormatError{"RAR compression is not supported. Install 'rar' and ensure it's in your system PATH."};
            }
            SET_TAR_FORMAT(a);
        
        case FORMAT::ARCHIVE::ACE:
        case FORMAT::ARCHIVE::ALZ:
        case FORMAT::ARCHIVE::ARC:
        case FORMAT::ARCHIVE::ARJ:
        case FORMAT::ARCHIVE::CAB:
        case FORMAT::ARCHIVE::DEB:
        case FORMAT::ARCHIVE::LHA:
        case FORMAT::ARCHIVE::RPM:
            archive_write_free(a);
            return FormatError{"The specified archive format is not supported for compression, only extraction is supported."};
    
        default:
            archive_write_free(a);
            return FormatError{"Unknown archive format."};
    }

    if (archive_write_open_filename(a, cmd.path_out.string().c_str()) != ARCHIVE_OK) {
        std::cerr << "Compression Error: Failed to open output package. " << archive_error_string(a) << std::endl;
        archive_write_free(a);
        return IOError{"Failed to open output package."};
    }

    auto start_time = std::chrono::steady_clock::now();
    uint64_t processed_bytes = 0;
    print_progress(0, total_bytes, start_time, true);

    if (fs::is_directory(cmd.path_in)) {
        for (const auto& dir_entry : fs::recursive_directory_iterator(cmd.path_in)) {
            if (fs::is_regular_file(dir_entry.path())) {
                fs::path relative_entry_path = fs::relative(dir_entry.path(), cmd.path_in.parent_path());
                write_file_to_archive(a, dir_entry.path(), relative_entry_path, total_bytes, processed_bytes, start_time);
            }
        }
    } else if (fs::is_regular_file(cmd.path_in)) {
        write_file_to_archive(a, cmd.path_in, cmd.path_in.filename(), total_bytes, processed_bytes, start_time);
    }
    
    print_progress(total_bytes, total_bytes, start_time, true);
    std::cout << "\n";

    archive_write_close(a);
    archive_write_free(a);
    return CompressionSuccess{};
}

static const char* passphrase_callback(struct archive *a, void *client_data) {
    static std::string passphrase;
    std::cout << "Enter password: ";
    std::cin >> passphrase;
    return passphrase.c_str();
}

bool decompress(const ComandCompress& cmd) {
    if (!fs::exists(cmd.path_in)) {
        std::cerr << "Decompression Error: Target archive does not exist." << std::endl;
        return false;
    }

    struct archive* a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    struct archive* ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL);

    if (cmd.encryption && !cmd.encryption_pass.empty()) {
        archive_read_add_passphrase(a, cmd.encryption_pass.c_str());
    }

    archive_read_set_passphrase_callback(a, nullptr, passphrase_callback);

    if (archive_read_open_filename(a, cmd.path_in.string().c_str(), 10240) != ARCHIVE_OK) {
        std::cerr << "Decompression Error: Cannot open file. " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    fs::create_directories(cmd.path_out);

    struct archive_entry* entry;
    int result;
    bool success = true;

    while ((result = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        fs::path original_path(archive_entry_pathname(entry));
        fs::path absolute_target_path = cmd.path_out / original_path;

        archive_entry_set_pathname(entry, absolute_target_path.string().c_str());

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            fs::create_directories(absolute_target_path);
            continue;
        }

        if (archive_write_header(ext, entry) == ARCHIVE_OK) {
            if (copy_data_stream(a, ext)  != ARCHIVE_OK) {
                std::cerr << "Decompression Warning: Error extracting data streams for " << original_path << std::endl;
                success = false;
            }
        }
        archive_write_finish_entry(ext);
    }

    if (result ==  ARCHIVE_FATAL) {
        std::cerr << "Decompression Error: Fatal processing failure. " << archive_error_string(a) << std::endl;
        success = false;
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    return success;
}
