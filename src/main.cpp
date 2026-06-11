#include "EngineCompress.h"
#include "conversions.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

void printBanner() {
    std::cout << "=================================================\n"
              << " Quark - Universal Archiver & Converter\n"
              << " Version: 1.0.0\n"
              << " Developed by: NovaFenice\n"
              << "=================================================\n\n";
}

void printUsage() {
    printBanner();
    std::cout << "Usage: qk [option] [arguments]\n\n"
              << "Options:\n"
              << "  --compress -<format> <file_or_path>   Compress a file or directory into the specified archive format.\n"
              << "  --help, -h                            Display this help message.\n"
              << "  --version, -v                         Display version and author information.\n\n"
              << "Example:\n"
              << "  qk --compress -tar.7z my_folder\n";
}

void printHelp() {
    printBanner();
    std::cout << "Usage: qk [option] [arguments]\n\n"
              << "Options:\n"
              << "  --archives                            Display all supported archive formats.\n"
              << "  --videos                              Display all supported video formats.\n"
              << "  --audio                               Display all supported audio formats.\n"
              << "  --images                              Display all supported image formats.\n"
              << "  --documents                           Display all supported document formats.\n"
              << "  --presentations                       Display all supported presentation formats.\n"
              << "  --ebooks                              Display all supported ebook formats.\n"
              << "  --fonts                               Display all supported font formats.\n"
              << "  -v, --version                         Display version and author information.\n"
              << "  -h, --help                            Display this help message.\n";
}

int handleCompress(int argc, char *argv[]) {
    if (argc < 4 || argc > 6) {
        std::cerr << "Error: Invalid number of arguments for --compress.\n";
        std::cerr << "Usage: qk --compress <input_path> -<format> [output_path] [-encrypt]\n";
        return 1;
    }

    ComandCompress cmd;
    cmd.path_in = fs::absolute(argv[2]);

    std::string_view fmt_arg = argv[3];
    if (fmt_arg.empty() || fmt_arg[0] != '-') {
        std::cerr << "Error: Format must start with '-' (e.g., -zip).\n";
        return 1;
    }
    std::string_view ext_str = fmt_arg.substr(1);
    cmd.ext = static_cast<FORMAT::ARCHIVE>(fnv1a(ext_str));

    cmd.encryption = false;
    bool has_output = false;

    if (argc >= 5) {
        std::string_view arg4 = argv[4];
        if (arg4 == "-encrypt") {
            cmd.encryption = true;
        } else {
            cmd.path_out = fs::absolute(arg4);
            has_output = true;
        }
    }

    if (argc == 6) {
        std::string_view arg5 = argv[5];
        if (arg5 == "-encrypt") {
            if (cmd.encryption) {
                std::cerr << "Error: Duplicate -encrypt argument.\n";
                return 1;
            }
            cmd.encryption = true;
        } else {
            std::cerr << "Error: Invalid 6th argument, expected -encrypt.\n";
            return 1;
        }
    }

    if (!has_output) {
        cmd.path_out = fs::absolute("Project." + std::string(ext_str));
    }

    if (cmd.encryption) {
        std::cout << "Enter password: ";
        std::getline(std::cin, cmd.encryption_pass);
        if (cmd.encryption_pass.empty()) {
            std::getline(std::cin, cmd.encryption_pass);
        }
    }

    auto result = compress(cmd);
    if (std::holds_alternative<CompressionSuccess>(result)) {
        std::cout << "Compression successful.\n";
        std::cout << "Output file: " << cmd.path_out.string() << "\n";
        return 0;
    } else if (std::holds_alternative<FormatError>(result)) {
        std::cerr << "Compression failed: " << std::get<FormatError>(result).message << "\n";
        return 1;
    } else if (std::holds_alternative<IOError>(result)) {
        std::cerr << "Compression failed: " << std::get<IOError>(result).message << "\n";
        return 1;
    }

    return 1;
}

int handleListFormats(uint32_t cmd_hash) {
    struct CmdMapping {
        uint32_t cmd_hash;
        uint32_t first;
        uint32_t last;
        std::string_view title;
    };

    static constexpr CmdMapping lookup_table[] = {
        {fnv1a("--archives"), static_cast<uint32_t>(FORMAT::ARCHIVE::_7Z), static_cast<uint32_t>(FORMAT::ARCHIVE::ZIP), "--- ARCHIVE LIST  ---"},
        {fnv1a("--videos"), static_cast<uint32_t>(FORMAT::VIDEO::_3G2), static_cast<uint32_t>(FORMAT::VIDEO::XVID), "--- VIDEO LIST  ---"},
        {fnv1a("--audio"), static_cast<uint32_t>(FORMAT::AUDIO::_8SVX), static_cast<uint32_t>(FORMAT::AUDIO::XA), "--- AUDIO LIST  ---"},
        {fnv1a("--images"), static_cast<uint32_t>(FORMAT::IMAGE::_3FR), static_cast<uint32_t>(FORMAT::IMAGE::YUV), "--- IMAGE LIST  ---"},
        {fnv1a("--documents"), static_cast<uint32_t>(FORMAT::DOCUMENT::ABW), static_cast<uint32_t>(FORMAT::DOCUMENT::XPS), "--- DOCUMENT LIST  ---"},
        {fnv1a("--presentations"), static_cast<uint32_t>(FORMAT::PRESENTATION::KEY), static_cast<uint32_t>(FORMAT::PRESENTATION::PPTX), "--- PRESENTATION LIST  ---"},
        {fnv1a("--ebooks"), static_cast<uint32_t>(FORMAT::EBOOK::AZW3), static_cast<uint32_t>(FORMAT::EBOOK::MOBI), "--- EBOOK LIST  ---"},
        {fnv1a("--fonts"), static_cast<uint32_t>(FORMAT::FONT::AFM), static_cast<uint32_t>(FORMAT::FONT::WOFF), "--- FONT LIST  ---"}
    };

    const CmdMapping *matched = nullptr;
    for (const auto &item : lookup_table) {
        if (item.cmd_hash == cmd_hash) {
            matched = &item;
            break;
        }
    }
    
    if (!matched) return 1;

    const auto range = FORMAT::getCategoryRange(matched->first, matched->last);

    std::cout << matched->title << "\n\n";

    constexpr int COLUMNS = 3;
    constexpr int COLUMN_WIDTH = 20;

    int current_col = 0;
    int index = 1;

    for (const auto *ptr = range.begin_ptr; ptr != range.end_ptr; ++ptr) {
        std::string index_str = (index < 10) ? (" " + std::to_string(index)) : std::to_string(index);
        std::string item_str = index_str + ") " + std::string(ptr->name);

        std::cout << std::left << std::setw(COLUMN_WIDTH) << item_str;

        current_col++;
        index++;

        if (current_col == COLUMNS) {
            std::cout << '\n';
            current_col = 0;
        }
    }

    if (current_col != 0) {
        std::cout << '\n';
    }

    std::cout << '\n';
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printUsage();
        return 0;
    }

    std::string_view first_arg = argv[1];
    uint32_t cmd_hash = fnv1a(first_arg);

    switch (cmd_hash) {
        case fnv1a("--compress"):
            return handleCompress(argc, argv);

        case fnv1a("--help"):
        case fnv1a("-h"):
            printHelp();
            return 0;

        case fnv1a("--version"):
        case fnv1a("-v"):
            printBanner();
            return 0;

        case fnv1a("--archives"):
        case fnv1a("--videos"):
        case fnv1a("--audio"):
        case fnv1a("--images"):
        case fnv1a("--documents"):
        case fnv1a("--presentations"):
        case fnv1a("--ebooks"):
        case fnv1a("--fonts"):
            return handleListFormats(cmd_hash);

        default:
            std::cout << "Error: Unknown option '" << first_arg << "'\nRun 'qk --help' for usage.\n";
            return 1;
    }
}