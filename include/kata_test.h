#ifndef KATA_TEST_H
#define KATA_TEST_H

#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <utility>

static const char* _kata_test_name = "";

template <typename T, typename = void>
struct _kata_printable : std::false_type {};

template <typename T>
struct _kata_printable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>> : std::true_type {};

template <typename T>
void _kata_print_val(std::ostream& os, const T& val, const char* label) {
    if constexpr (_kata_printable<T>::value) {
        os << label << val << "\n";
    }
}

template <typename T>
void _kata_print_raw(std::ostream& os, const T& val) {
    if constexpr (_kata_printable<T>::value) {
        os << val;
    }
}

#define TEST(name) do { _kata_test_name = name; std::cout << "  " << name << "\n" << std::flush; } while (0)

#define CHECK(cond)                                                         \
    do {                                                                    \
        if (!(cond)) {                                                      \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << #cond << std::endl;                                \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#define EQL(expected, actual)                                               \
    do {                                                                    \
        auto&& _e = (expected);                                             \
        auto&& _a = (actual);                                               \
        if (!(_a == _e)) {                                                  \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << #actual << " == " << #expected << "\n";            \
            _kata_print_val(std::cerr, _a, "    actual:   ");              \
            _kata_print_val(std::cerr, _e, "    expected: ");              \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#define VAL(opt, expected)                                                  \
    do {                                                                    \
        auto&& _o = (opt);                                                  \
        if (!_o.has_value()) {                                              \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << #opt << " is empty (expected ";                    \
            _kata_print_raw(std::cerr, (expected));                         \
            std::cerr << ")\n";                                             \
            std::abort();                                                   \
        }                                                                   \
        auto&& _e = (expected);                                             \
        if (!(*_o == _e)) {                                                 \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << "*" << #opt << " == " << #expected << "\n";        \
            _kata_print_val(std::cerr, *_o, "    actual:   ");             \
            _kata_print_val(std::cerr, _e, "    expected: ");              \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#endif
