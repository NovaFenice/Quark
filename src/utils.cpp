#include "utils.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch (const std::bad_alloc&) {
        return 0;
    }
}

utils::UpdateResult utils::check_for_updates() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return utils::NetworkError{"Failed to initialize cURL"};
    }

    std::string readBuffer;

    curl_easy_setopt(curl, CURLOPT_URL, GITHUB_API_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1500L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "User-Agent: Quark-UpdateChecker");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return utils::NetworkError{"Network error"};

    try {
        auto data = json::parse(readBuffer);
        json latest_release = data.is_array() ? data[0] : data;

        std::string latest_version = latest_release.value("tag_name", "");
        if (latest_version.empty()) latest_version = latest_release.value("name", "");
        std::string download_url = latest_release.value("html_url", "");

        if (latest_version.empty()) return utils::AlreadyUpdated{};

        if (!latest_version.empty() && latest_version[0] == 'v') latest_version.erase(0, 1);

        if (latest_version != CURRENT_VERSION) {
            return utils::UpdateAvailable{latest_version, download_url};
        }
        return utils::AlreadyUpdated{};
    } catch (...) {
        return utils::AlreadyUpdated{};
    }
}

void credits::printBanner() {

    auto check_for_updates = std::async(std::launch::async, utils::check_for_updates);

    std::cout << style::colorize(style::BRIGHT_YELLOW, NAME) << "\n";
    std::cout << style::BRIGHT_BLACK << "---------------------------------------------------------\n" << style::RESET;
    std::cout << "  Author:  " << style::colorize(style::BRIGHT_WHITE, AUTHOR) << "\n";
    std::cout << "  GitHub:  " << style::colorize(style::BRIGHT_WHITE, GITHUB) << "\n";
    std::cout << "  Version: " << style::colorize(style::BRIGHT_WHITE, utils::CURRENT_VERSION);

    if (check_for_updates.wait_for(std::chrono::milliseconds(150)) == std::future_status::ready) {
        auto update_status = check_for_updates.get();
        
        if (std::holds_alternative<utils::UpdateAvailable>(update_status)) {
            auto update_info = std::get<utils::UpdateAvailable>(update_status);
            std::cout << " -> " << style::colorize(style::BRIGHT_GREEN, update_info.new_version)
                      << " " << style::colorize(style::ITALIC, "(Update Available!)");
        }
    }

    std::cout << "\n";
    std::cout << style::BRIGHT_BLACK << "---------------------------------------------------------\n" << style::RESET;
}