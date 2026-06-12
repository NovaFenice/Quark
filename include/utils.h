#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <future>

using json = nlohmann::json;

namespace utils {
    inline constexpr const char* CURRENT_VERSION = "1.0.0";
    inline constexpr const char* GITHUB_API_URL = "https://api.github.com/repos/NovaFenice/Quark/tags";

    struct UpdateAvailable { std::string new_version; std::string download_url; };
    struct AlreadyUpdated  {};
    struct NetworkError    { std::string message; };

    using UpdateResult = std::variant<UpdateAvailable, AlreadyUpdated, NetworkError>;

    UpdateResult check_for_updates();
}

namespace style {
    // STYLE MODIFIERS
    inline constexpr const char* RESET          = "\033[0m";
    inline constexpr const char* BOLD           = "\033[1m";
    inline constexpr const char* FAINT          = "\033[2m";
    inline constexpr const char* ITALIC         = "\033[3m";
    inline constexpr const char* UNDERLINE      = "\033[4m";
    inline constexpr const char* BLINK          = "\033[5m";
    inline constexpr const char* REVERSE        = "\033[7m";

    // STANDARD COLORS
    inline constexpr const char* BLACK          = "\033[30m";
    inline constexpr const char* RED            = "\033[31m";
    inline constexpr const char* GREEN          = "\033[32m";
    inline constexpr const char* YELLOW         = "\033[33m";
    inline constexpr const char* BLUE           = "\033[34m";
    inline constexpr const char* MAGENTA        = "\033[35m";
    inline constexpr const char* CYAN           = "\033[36m";
    inline constexpr const char* WHITE          = "\033[37m";

    // GLOWING COLORS
    inline constexpr const char* BRIGHT_BLACK   = "\033[90m";
    inline constexpr const char* BRIGHT_RED     = "\033[91m";
    inline constexpr const char* BRIGHT_GREEN   = "\033[92m";
    inline constexpr const char* BRIGHT_YELLOW  = "\033[93m";
    inline constexpr const char* BRIGHT_BLUE    = "\033[94m";
    inline constexpr const char* BRIGHT_MAGENTA = "\033[95m";
    inline constexpr const char* BRIGHT_CYAN    = "\033[96m";
    inline constexpr const char* BRIGHT_WHITE   = "\033[97m";

    // BACKGROUND COLORS
    inline constexpr const char* BG_BLACK       = "\033[40m";
    inline constexpr const char* BG_RED         = "\033[41m";
    inline constexpr const char* BG_GREEN       = "\033[42m";
    inline constexpr const char* BG_YELLOW      = "\033[43m";
    inline constexpr const char* BG_BLUE        = "\033[44m";
    inline constexpr const char* BG_MAGENTA     = "\033[45m";
    inline constexpr const char* BG_CYAN        = "\033[46m";
    inline constexpr const char* BG_WHITE       = "\033[47m";

    // GLOWING BACKGROUND COLORS
    inline constexpr const char* BG_BRIGHT_BLACK   = "\033[100m";
    inline constexpr const char* BG_BRIGHT_RED     = "\033[101m";
    inline constexpr const char* BG_BRIGHT_GREEN   = "\033[102m";
    inline constexpr const char* BG_BRIGHT_YELLOW  = "\033[103m";
    inline constexpr const char* BG_BRIGHT_BLUE    = "\033[104m";
    inline constexpr const char* BG_BRIGHT_MAGENTA = "\033[105m";
    inline constexpr const char* BG_BRIGHT_CYAN    = "\033[106m";
    inline constexpr const char* BG_BRIGHT_WHITE   = "\033[107m";

    inline std::string colorize(std::string_view color, std::string_view text) {
        std::string result;
        result.reserve(color.size() + text.size() + 4);
        result.append(color).append(text).append(RESET);
        return result;
    }
}

namespace credits {
    inline constexpr const char* NAME = R"(
       ____  __  _____    ____  __ __
      / __ \/ / / /   |  / __ \/ //_/
     / / / / / / / /| | / /_/ / , <   
    / /_/ / /_/ / ___ |/ _, _/ /| |  
    \___\_\____/_/  |_/_/ |_/_/ |_|  
    )";
    inline constexpr const char* AUTHOR = "NovaFenice";
    inline constexpr const char* GITHUB = "https://github.com/NovaFenice/Quark";
    inline constexpr const char* VERSION = utils::CURRENT_VERSION;

    void printBanner();
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);