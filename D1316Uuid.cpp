#include "D1316Uuid.h"
#include <string>
#include <string_view>
#include <unordered_set>

namespace {
    inline std::string normalize(std::string_view input) {
        if (input.empty()) {
            return {};
        }

        std::string out;
        out.reserve(32);

        for (char ch : input) {
            if (ch == '-') {
                continue;
            }
            unsigned char c = static_cast<unsigned char>(ch);
            if (std::isxdigit(c)) {
                out += static_cast<char>(std::tolower(c));
            }
        }

        if (out.size() != 32) {
            return {};
        }

        if (out[12] != '4') {
            return {};
        }
        
        char variant = out[16];
        if (variant != '8' && variant != '9' && variant != 'a' && variant != 'b') {
            return {};
        }

        out.shrink_to_fit();
        return out;
    }

    static std::unordered_set<std::string> listed;
}

namespace D1316Uuid {
    EXPORT void init(const char* uuids[], int count) {
        listed.clear();
        
        if (!uuids || count <= 0) {
            return;
        }

        listed.reserve(static_cast<size_t>(count));

        for (int i = 0; i < count; ++i) {
            if (!uuids[i]) {
                continue;
            }

            std::string norm = normalize(uuids[i]);
            if (!norm.empty()) {
                listed.emplace(std::move(norm));
            }
        }
    }

    EXPORT bool check(std::string_view s) {
        if (listed.empty()) {
            return false;
        }
        auto n = normalize(s);
        return !n.empty() && listed.count(n);
    }
}

extern "C" {
    EXPORT void D1316Uuid_Init(const char* uuids[], int count) {
        D1316Uuid::init(uuids, count);
    }

    EXPORT bool D1316Uuid_Check(const char* uuid) {
        return D1316Uuid::check(uuid ? uuid : "");
    }
}
