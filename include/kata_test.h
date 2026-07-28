#ifndef KATA_TEST_H
#define KATA_TEST_H

#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <utility>
#include <optional>
#include <vector>
#include <cstddef>
#include <string>

static const char* _kata_test_name = "";

// --- type traits ------------------------------------------------------------

template <typename T> struct _kata_is_std_vector : std::false_type {};
template <typename T, typename A>
struct _kata_is_std_vector<std::vector<T, A>> : std::true_type {};

template <typename T> struct _kata_is_std_optional : std::false_type {};
template <typename T>
struct _kata_is_std_optional<std::optional<T>> : std::true_type {};

template <typename T, typename = void>
struct _kata_is_tree_node : std::false_type {};
template <typename T>
struct _kata_is_tree_node<T, std::void_t<
    decltype(std::declval<const T&>().val),
    decltype(std::declval<const T&>().left),
    decltype(std::declval<const T&>().right)
>> : std::true_type {};

template <typename T, typename = void>
struct _kata_is_list_node : std::false_type {};
template <typename T>
struct _kata_is_list_node<T, std::void_t<
    decltype(std::declval<const T&>().val),
    decltype(std::declval<const T&>().next)
>> : std::true_type {};

template <typename T, typename = void>
struct _kata_is_point : std::false_type {};
template <typename T>
struct _kata_is_point<T, std::void_t<
    decltype(std::declval<const T&>().x),
    decltype(std::declval<const T&>().y)
>> : std::true_type {};

template <typename T, typename = void>
struct _kata_is_edge : std::false_type {};
template <typename T>
struct _kata_is_edge<T, std::void_t<
    decltype(std::declval<const T&>().to),
    decltype(std::declval<const T&>().weight)
>> : std::true_type {};

template <typename T> struct _kata_is_std_pair : std::false_type {};
template <typename A, typename B>
struct _kata_is_std_pair<std::pair<A, B>> : std::true_type {};

template <typename T>
constexpr bool _kata_is_complex() {
    return _kata_is_std_vector<T>::value
        || _kata_is_std_optional<T>::value
        || (std::is_pointer_v<T>
            && (_kata_is_tree_node<std::remove_pointer_t<T>>::value
                || _kata_is_list_node<std::remove_pointer_t<T>>::value))
        || _kata_is_point<T>::value
        || _kata_is_edge<T>::value;
}

// --- pretty printer ---------------------------------------------------------

inline void _kata_indent(std::ostream& os, int n) {
    for (int i = 0; i < n; ++i) os << "  ";
}

template <typename T>
void _kata_pretty(std::ostream& os, const T& v, int depth = 0) {
    using U = std::remove_cv_t<std::remove_reference_t<T>>;
    if constexpr (_kata_is_std_vector<U>::value) {
        os << "vector (size " << v.size() << ")";
        for (std::size_t i = 0; i < v.size(); ++i) {
            os << "\n";
            _kata_indent(os, depth + 1);
            os << "[" << i << "] ";
            _kata_pretty(os, v[i], depth + 1);
        }
    } else if constexpr (_kata_is_std_optional<U>::value) {
        if (v.has_value()) {
            os << "optional(";
            _kata_pretty(os, *v, depth);
            os << ")";
        } else {
            os << "null";
        }
    } else if constexpr (std::is_pointer_v<U> && _kata_is_tree_node<std::remove_pointer_t<U>>::value) {
        if (v == nullptr) {
            os << "null";
        } else {
            os << "(" << v->val << " ";
            _kata_pretty(os, v->left, depth);
            os << " ";
            _kata_pretty(os, v->right, depth);
            os << ")";
        }
    } else if constexpr (std::is_pointer_v<U> && _kata_is_list_node<std::remove_pointer_t<U>>::value) {
        if (v == nullptr) {
            os << "null";
        } else {
            os << v->val;
            if (v->next) {
                os << " -> ";
                _kata_pretty(os, v->next, depth);
            }
        }
    } else if constexpr (_kata_is_point<U>::value) {
        os << "(" << v.x << ", " << v.y << ")";
    } else if constexpr (_kata_is_edge<U>::value) {
        os << "(to=" << v.to << ", w=" << v.weight << ")";
    } else if constexpr (_kata_is_std_pair<U>::value) {
        os << "("; _kata_pretty(os, v.first, depth);
        os << ", "; _kata_pretty(os, v.second, depth);
        os << ")";
    } else if constexpr (std::is_same_v<U, std::nullptr_t>) {
        os << "null";
    } else if constexpr (std::is_same_v<U, bool>) {
        os << (v ? "true" : "false");
    } else {
        os << v;
    }
}

// --- diff detection ---------------------------------------------------------

template <typename N>
void _kata_tree_diff(std::ostream& os, N* expected, N* actual, const std::string& path) {
    if (expected == nullptr && actual == nullptr) return;
    const char* where = path.empty() ? "root" : path.c_str();
    if (expected == nullptr) {
        os << "  first diff at " << where
           << " (expected null, got " << actual->val << ")\n";
        return;
    }
    if (actual == nullptr) {
        os << "  first diff at " << where
           << " (expected " << expected->val << ", got null)\n";
        return;
    }
    if (!(expected->val == actual->val)) {
        os << "  first diff at " << where
           << " (expected " << expected->val
           << ", got " << actual->val << ")\n";
        return;
    }
    _kata_tree_diff(os, expected->left, actual->left, path + "L");
    _kata_tree_diff(os, expected->right, actual->right, path + "R");
}

template <typename N>
void _kata_list_diff(std::ostream& os, N* expected, N* actual, std::size_t pos) {
    if (expected == nullptr && actual == nullptr) return;
    if (expected == nullptr) {
        os << "  first diff at position " << pos
           << " (expected null, got " << actual->val << ")\n";
        return;
    }
    if (actual == nullptr) {
        os << "  first diff at position " << pos
           << " (expected " << expected->val << ", got null)\n";
        return;
    }
    if (!(expected->val == actual->val)) {
        os << "  first diff at position " << pos
           << " (expected " << expected->val
           << ", got " << actual->val << ")\n";
        return;
    }
    _kata_list_diff(os, expected->next, actual->next, pos + 1);
}

template <typename T, typename U>
auto _kata_diff_any(std::ostream& os, const T& e, const U& a)
    -> std::enable_if_t<std::is_same_v<T, U> && !std::is_same_v<T, std::nullptr_t>>;

template <typename T, typename U>
auto _kata_diff_any(std::ostream& os, const T&, const U&)
    -> std::enable_if_t<!std::is_same_v<T, U> || std::is_same_v<T, std::nullptr_t>>;

template <typename T>
void _kata_diff(std::ostream& os, const T& expected, const T& actual) {
    using U = std::remove_cv_t<std::remove_reference_t<T>>;
    if constexpr (_kata_is_std_vector<U>::value) {
        if (expected.size() != actual.size()) {
            os << "  size mismatch: expected " << expected.size()
               << ", got " << actual.size() << "\n";
            return;
        }
        for (std::size_t i = 0; i < expected.size(); ++i) {
            if (!(expected[i] == actual[i])) {
                using E = std::remove_cv_t<std::remove_reference_t<decltype(expected[i])>>;
                if constexpr (_kata_is_complex<E>()) {
                    os << "  first diff at index " << i << ":\n";
                    os << "    expected: ";
                    _kata_pretty(os, expected[i], 2);
                    os << "\n";
                    os << "    actual:   ";
                    _kata_pretty(os, actual[i], 2);
                    os << "\n";
                } else {
                    os << "  first diff at index " << i
                       << " (expected ";
                    _kata_pretty(os, expected[i], 0);
                    os << ", got ";
                    _kata_pretty(os, actual[i], 0);
                    os << ")\n";
                }
                return;
            }
        }
    } else if constexpr (_kata_is_std_optional<U>::value) {
        if (expected.has_value() != actual.has_value()) {
            os << "  presence differs: expected "
               << (expected.has_value() ? "some" : "null")
               << ", got "
               << (actual.has_value() ? "some" : "null") << "\n";
        } else if (expected.has_value() && !(*expected == *actual)) {
            os << "  contents differ\n";
        }
    } else if constexpr (std::is_pointer_v<U> && _kata_is_tree_node<std::remove_pointer_t<U>>::value) {
        _kata_tree_diff(os, expected, actual, std::string());
    } else if constexpr (std::is_pointer_v<U> && _kata_is_list_node<std::remove_pointer_t<U>>::value) {
        _kata_list_diff(os, expected, actual, 0);
    } else if constexpr (_kata_is_point<U>::value) {
        if (!(expected.x == actual.x))
            os << "  first diff at x (expected " << expected.x
               << ", got " << actual.x << ")\n";
        else if (!(expected.y == actual.y))
            os << "  first diff at y (expected " << expected.y
               << ", got " << actual.y << ")\n";
    } else if constexpr (_kata_is_edge<U>::value) {
        if (!(expected.to == actual.to))
            os << "  first diff at to (expected " << expected.to
               << ", got " << actual.to << ")\n";
        else if (!(expected.weight == actual.weight))
            os << "  first diff at weight (expected " << expected.weight
               << ", got " << actual.weight << ")\n";
    } else if constexpr (_kata_is_std_pair<U>::value) {
        if (!(expected.first == actual.first)) {
            os << "  first diff at first (expected ";
            _kata_pretty(os, expected.first, 0);
            os << ", got ";
            _kata_pretty(os, actual.first, 0);
            os << ")\n";
        } else if (!(expected.second == actual.second)) {
            os << "  first diff at second (expected ";
            _kata_pretty(os, expected.second, 0);
            os << ", got ";
            _kata_pretty(os, actual.second, 0);
            os << ")\n";
        }
    }
}

template <typename T, typename U>
auto _kata_diff_any(std::ostream& os, const T& e, const U& a)
    -> std::enable_if_t<std::is_same_v<T, U> && !std::is_same_v<T, std::nullptr_t>> {
    _kata_diff(os, e, a);
}

template <typename T, typename U>
auto _kata_diff_any(std::ostream& os, const T&, const U&)
    -> std::enable_if_t<!std::is_same_v<T, U> || std::is_same_v<T, std::nullptr_t>> {
}

// --- macros -----------------------------------------------------------------

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
            std::cerr << "    expected: ";                                  \
            _kata_pretty(std::cerr, _e, 2);                                 \
            std::cerr << "\n";                                              \
            std::cerr << "    actual:   ";                                  \
            _kata_pretty(std::cerr, _a, 2);                                 \
            std::cerr << "\n";                                              \
            _kata_diff_any(std::cerr, _e, _a);                              \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#define VAL(opt, expected)                                                  \
    do {                                                                    \
        auto&& _o = (opt);                                                  \
        if (!_o.has_value()) {                                              \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << #opt << " is empty (expected ";                    \
            _kata_pretty(std::cerr, (expected), 0);                         \
            std::cerr << ")\n";                                             \
            std::abort();                                                   \
        }                                                                   \
        auto&& _e = (expected);                                             \
        if (!(*_o == _e)) {                                                 \
            std::cerr << "  FAIL [" << (_kata_test_name ? _kata_test_name : "?") << "]: " \
                      << "*" << #opt << " == " << #expected << "\n";        \
            std::cerr << "    expected: ";                                  \
            _kata_pretty(std::cerr, _e, 1);                                 \
            std::cerr << "\n";                                              \
            std::cerr << "    actual:   ";                                  \
            _kata_pretty(std::cerr, *_o, 1);                                \
            std::cerr << "\n";                                              \
            _kata_diff(std::cerr, _e, *_o);                                 \
            std::abort();                                                   \
        }                                                                   \
    } while (0)

#endif
