#pragma once

#include <string>
#include <string_view>

#if defined(_WIN32) || defined(_WIN64)
    #define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
    #define EXPORT __attribute__((visibility("default")))
#endif

namespace D1316Uuid {
    EXPORT void init(const char* uuids[], int count);
    EXPORT bool check(std::string_view s);
}

extern "C" {
    EXPORT void D1316Uuid_Init(const char* uuids[], int count);
    EXPORT bool D1316Uuid_Check(const char* uuid);
}
