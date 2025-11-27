#include "D1316Uuid.h"
#include <iostream>
#include <iomanip>

int main() {
    const char* whitelist[] = {
        "f47ac10b-58cc4372-a567-0e02b2c3d479",
        "a0eebc99-9c0b-4ef8bb6d-6bb9bd380a11",
        "550e8400-e29b-41d4-a716446655440000",
        "6BA7B810-9DAD-41D1-80B4-00C04FD430C8",
        "6ba7b8109dad41d180b400c04fd430c8",
        "00000000-0000-4000-8000-000000000000",
        "ffffffffffff4fffbfffffffffffffff",
    };

#ifdef D1316UUID_TEST_UNIT
    D1316Uuid::init(whitelist, 7);
#else
    D1316Uuid_Init(whitelist, 7);
#endif

    int passed = 0, total = 0;
    auto test = [&](const char* input, bool expect) {
        total++;
#ifdef D1316UUID_TEST_UNIT
        bool result = D1316Uuid::check(input);
#else
        bool result = D1316Uuid_Check(input);
#endif
        bool ok = result == expect;
        if (ok) {
            passed++;
        }
        std::cout << std::left << std::setw(48) << (input ? input : "(nullptr)")
                  << " -> " << (result ? "OK  " : "NO  ")
                  << (ok ? "[PASS]" : "[FAIL]") << '\n';
    };

    test("f47ac10b-58cc-4372-a567-0e02b2c3d479", true);
    test("F47AC10B58CC4372A5670E02B2C3D479", true);
    test("f47ac10b58cc4372a5670e02b2c3d479", true);
    test("123e4567-e89b-12d3-a456-426614174000", false);
    test("123E4567E89B12D3A456426614174000", false);
    test("a0eebc999c0b4ef8bb6d6bb9bd380a11", true);
    test("00000000-0000-4000-8000-000000000000", true);
    test("FFFFFFFF-FFFF-4FFF-BFFF-FFFFFFFFFFFF", true);

    test("g47ac10b-58cc-4372-a567-0e02b2c3d479", false);
    test("123e4567-e89b-12d3-a456-42661417400", false);
    test("123e4567-e89b-12d3-a456-4266141740000", false);
    test("123e4567-e89b-02d3-a456-426614174000", false);
    test("123e4567-e89b-12d3-c456-426614174000", false);
    test("66666666-6666-5666-9666-666666666666", false);
    test("", false);
    test("   ", false);
    test("hello world", false);
#ifndef D1316UUID_TEST_UNIT
    test(nullptr, false);
#endif

    std::cout << "\n====================================================\n";
    if (passed == total) {
        std::cout << "TEST PASSED\n";
        return 0;
    } else {
        std::cout << "TEST FAILED\n";
        return 1;
    }
}
