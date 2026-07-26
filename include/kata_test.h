#ifndef KATA_TEST_H
#define KATA_TEST_H

#include <iostream>
#include <cstdlib>

static const char* _kata_test_name = "";

#define TEST(name) do { _kata_test_name = name; } while (0)

#define CHECK(cond)                                                         \
    do {                                                                    \
        if (!(cond)) {                                                      \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << #cond << std::endl;                                \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#endif
