#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace kata {

// --- common types -----------------------------------------------------------

struct Op {
    std::string op;
    std::vector<std::int64_t> iargs;
    std::vector<std::string> sargs;
};

enum class ReplyKind { Void, I64, F64, Empty, Bool, Str, Strs };

struct Reply {
    ReplyKind kind = ReplyKind::Void;
    std::int64_t i64 = 0;
    double f64 = 0;
    bool b = false;
    std::string s;
    std::vector<std::string> strs;

    static Reply void_reply() { return Reply{ReplyKind::Void}; }
    static Reply empty() {
        Reply r;
        r.kind = ReplyKind::Empty;
        return r;
    }
    static Reply from_i64(std::int64_t v) {
        Reply r;
        r.kind = ReplyKind::I64;
        r.i64 = v;
        return r;
    }
    static Reply from_f64(double v) {
        Reply r;
        r.kind = ReplyKind::F64;
        r.f64 = v;
        return r;
    }
    static Reply from_bool(bool v) {
        Reply r;
        r.kind = ReplyKind::Bool;
        r.b = v;
        return r;
    }
    static Reply from_str(std::string v) {
        Reply r;
        r.kind = ReplyKind::Str;
        r.s = std::move(v);
        return r;
    }
    static Reply from_strs(std::vector<std::string> v) {
        Reply r;
        r.kind = ReplyKind::Strs;
        r.strs = std::move(v);
        return r;
    }
};

struct Node {
    std::int64_t value = 0;
    Node *left = nullptr;
    Node *right = nullptr;
};

struct ListNode {
    std::int64_t value = 0;
    ListNode *next = nullptr;
};

struct Edge {
    int to = 0;
    std::int64_t weight = 0;
};

using AdjList = std::vector<std::vector<Edge>>;
using Matrix = std::vector<std::vector<std::int64_t>>;

struct Point {
    int x = 0;
    int y = 0;
};

// --- readers / writers ------------------------------------------------------

inline std::vector<std::int64_t> read_list_i64() {
    std::size_t n = 0;
    if (!(std::cin >> n)) {
        return {};
    }
    std::vector<std::int64_t> a(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    return a;
}

inline void write_list_i64(const std::vector<std::int64_t> &a) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (i) {
            std::cout << ' ';
        }
        std::cout << a[i];
    }
    std::cout << '\n';
}

inline std::vector<std::int64_t> read_bool_list_as_i64() {
    // 0/1 ints
    return read_list_i64();
}

inline std::string read_string_line() {
    std::string s;
    std::getline(std::cin, s);
    if (!s.empty() && s.back() == '\r') {
        s.pop_back();
    }
    return s;
}

inline std::string read_string() {
    std::size_t n = 0;
    std::cin >> n;
    std::cin.get(); // space or newline
    std::string s(n, '\0');
    if (n) {
        std::cin.read(&s[0], static_cast<std::streamsize>(n));
    }
    return s;
}

inline void write_string(const std::string &s) {
    std::cout << s.size() << ' ' << s << '\n';
}

inline Node *read_tree() {
    int flag = 0;
    if (!(std::cin >> flag)) {
        return nullptr;
    }
    if (!flag) {
        return nullptr;
    }
    auto *n = new Node();
    std::cin >> n->value;
    n->left = read_tree();
    n->right = read_tree();
    return n;
}

inline void write_tree(Node *n) {
    if (!n) {
        std::cout << "0\n";
        return;
    }
    std::cout << "1 " << n->value << '\n';
    write_tree(n->left);
    write_tree(n->right);
}

inline void free_tree(Node *n) {
    if (!n) {
        return;
    }
    free_tree(n->left);
    free_tree(n->right);
    delete n;
}

inline ListNode *read_list_nodes(int &cycle_to) {
    std::size_t n = 0;
    std::cin >> n;
    cycle_to = -1;
    if (n == 0) {
        std::cin >> cycle_to;
        return nullptr;
    }
    std::vector<ListNode *> nodes(n);
    for (std::size_t i = 0; i < n; ++i) {
        nodes[i] = new ListNode();
        std::cin >> nodes[i]->value;
    }
    for (std::size_t i = 0; i + 1 < n; ++i) {
        nodes[i]->next = nodes[i + 1];
    }
    std::cin >> cycle_to;
    if (cycle_to >= 0 && static_cast<std::size_t>(cycle_to) < n) {
        nodes[n - 1]->next = nodes[static_cast<std::size_t>(cycle_to)];
    }
    return nodes[0];
}

// Does not free if cycle exists — process ends after test.
inline void free_list_nodes(ListNode *head, bool has_cycle) {
    if (has_cycle || !head) {
        return;
    }
    while (head) {
        ListNode *n = head->next;
        delete head;
        head = n;
    }
}

inline AdjList read_adj_list() {
    std::size_t n = 0;
    std::cin >> n;
    AdjList g(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t m = 0;
        std::cin >> m;
        g[i].resize(m);
        for (std::size_t j = 0; j < m; ++j) {
            std::cin >> g[i][j].to >> g[i][j].weight;
        }
    }
    return g;
}

inline void write_adj_list(const AdjList &g) {
    std::cout << g.size() << '\n';
    for (const auto &row : g) {
        std::cout << row.size();
        for (const auto &e : row) {
            std::cout << ' ' << e.to << ' ' << e.weight;
        }
        std::cout << '\n';
    }
}

inline Matrix read_matrix() {
    std::size_t n = 0;
    std::cin >> n;
    Matrix m(n, std::vector<std::int64_t>(n));
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            std::cin >> m[i][j];
        }
    }
    return m;
}

inline std::vector<std::pair<std::int64_t, std::int64_t>> read_pairs() {
    std::size_t n = 0;
    std::cin >> n;
    std::vector<std::pair<std::int64_t, std::int64_t>> a(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> a[i].first >> a[i].second;
    }
    return a;
}

inline void write_pairs(const std::vector<std::pair<std::int64_t, std::int64_t>> &a) {
    std::cout << a.size() << '\n';
    for (const auto &p : a) {
        std::cout << p.first << ' ' << p.second << '\n';
    }
}

inline std::vector<std::vector<std::int64_t>> read_list_of_lists() {
    std::size_t n = 0;
    std::cin >> n;
    std::vector<std::vector<std::int64_t>> a(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t m = 0;
        std::cin >> m;
        a[i].resize(m);
        for (std::size_t j = 0; j < m; ++j) {
            std::cin >> a[i][j];
        }
    }
    return a;
}

inline void write_list_of_lists(const std::vector<std::vector<std::int64_t>> &a) {
    std::cout << a.size() << '\n';
    for (const auto &row : a) {
        std::cout << row.size();
        for (auto v : row) {
            std::cout << ' ' << v;
        }
        std::cout << '\n';
    }
}

inline void write_optional_list(const std::optional<std::vector<std::int64_t>> &a) {
    if (!a) {
        std::cout << "null\n";
        return;
    }
    write_list_i64(*a);
}

inline std::vector<Op> read_ops() {
    // optional init line: "init ..." as first token after count? handled by caller via init prefix
    std::size_t n = 0;
    if (!(std::cin >> n)) {
        return {};
    }
    std::string line;
    std::getline(std::cin, line); // rest of count line

    std::vector<Op> ops;
    ops.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        std::istringstream iss(line);
        Op op;
        if (!(iss >> op.op)) {
            continue;
        }
        std::string tok;
        while (iss >> tok) {
            if (!tok.empty() && tok.front() == '"') {
                std::string s = tok.substr(1);
                while (!s.empty() && s.back() != '"') {
                    std::string more;
                    if (!(iss >> more)) {
                        break;
                    }
                    s += ' ';
                    s += more;
                }
                if (!s.empty() && s.back() == '"') {
                    s.pop_back();
                }
                op.sargs.push_back(s);
            } else {
                // try int
                try {
                    std::size_t idx = 0;
                    auto v = std::stoll(tok, &idx);
                    if (idx == tok.size()) {
                        op.iargs.push_back(v);
                    } else {
                        op.sargs.push_back(tok);
                    }
                } catch (...) {
                    op.sargs.push_back(tok);
                }
            }
        }
        ops.push_back(std::move(op));
    }
    return ops;
}

inline std::vector<std::int64_t> read_init_i64() {
    // first line: init_n a1 a2 ...   OR just "0" for none
    std::size_t n = 0;
    if (!(std::cin >> n)) {
        return {};
    }
    std::vector<std::int64_t> a(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    return a;
}

inline void write_replies(const std::vector<Reply> &replies) {
    for (const auto &r : replies) {
        switch (r.kind) {
        case ReplyKind::Void:
            break;
        case ReplyKind::I64:
            std::cout << r.i64 << '\n';
            break;
        case ReplyKind::F64: {
            // integer-looking floats without trailing .0 noise when whole
            double v = r.f64;
            auto iv = static_cast<std::int64_t>(v);
            if (static_cast<double>(iv) == v) {
                std::cout << iv << '\n';
            } else {
                std::cout << v << '\n';
            }
            break;
        }
        case ReplyKind::Empty:
            std::cout << "empty\n";
            break;
        case ReplyKind::Bool:
            std::cout << (r.b ? "true" : "false") << '\n';
            break;
        case ReplyKind::Str:
            std::cout << r.s << '\n';
            break;
        case ReplyKind::Strs:
            std::cout << r.strs.size() << '\n';
            for (const auto &s : r.strs) {
                std::cout << s << '\n';
            }
            break;
        }
    }
}

// --- runners ----------------------------------------------------------------

template <class F>
int run_list_inplace(F fn) {
    auto a = read_list_i64();
    fn(a);
    write_list_i64(a);
    return 0;
}

template <class F>
int run_commands(F fn) {
    auto init = read_init_i64();
    auto ops = read_ops();
    auto replies = fn(init, ops);
    write_replies(replies);
    return 0;
}

// list + needle -> bool
template <class F>
int run_search(F fn) {
    auto a = read_list_i64();
    std::int64_t needle = 0;
    std::cin >> needle;
    bool ok = fn(a, needle);
    std::cout << (ok ? "true" : "false") << '\n';
    return 0;
}

// list -> scalar
template <class F>
int run_list_scalar(F fn) {
    auto a = read_list_i64();
    std::cout << fn(a) << '\n';
    return 0;
}

// list + k -> scalar
template <class F>
int run_list_k_scalar(F fn) {
    auto a = read_list_i64();
    std::int64_t k = 0;
    std::cin >> k;
    std::cout << fn(a, k) << '\n';
    return 0;
}

// list -> list
template <class F>
int run_list_list(F fn) {
    auto a = read_list_i64();
    write_list_i64(fn(a));
    return 0;
}

// list + k -> list
template <class F>
int run_list_k_list(F fn) {
    auto a = read_list_i64();
    std::int64_t k = 0;
    std::cin >> k;
    write_list_i64(fn(a, k));
    return 0;
}

// pairs -> pairs
template <class F>
int run_pairs(F fn) {
    auto a = read_pairs();
    write_pairs(fn(a));
    return 0;
}

// amount + coins -> scalar
template <class F>
int run_coin(F fn) {
    std::int64_t amount = 0;
    std::cin >> amount;
    auto coins = read_list_i64();
    std::cout << fn(amount, coins) << '\n';
    return 0;
}

// capacity + weights + values -> scalar
template <class F>
int run_knapsack(F fn) {
    std::int64_t cap = 0;
    std::cin >> cap;
    auto w = read_list_i64();
    auto v = read_list_i64();
    std::cout << fn(cap, w, v) << '\n';
    return 0;
}

// two length-prefixed strings -> scalar
template <class F>
int run_str2_scalar(F fn) {
    auto a = read_string();
    auto b = read_string();
    std::cout << fn(a, b) << '\n';
    return 0;
}

// tree -> list
template <class F>
int run_tree_list(F fn) {
    Node *t = read_tree();
    write_list_i64(fn(t));
    free_tree(t);
    return 0;
}

// tree + needle -> bool
template <class F>
int run_tree_find(F fn) {
    Node *t = read_tree();
    std::int64_t needle = 0;
    std::cin >> needle;
    bool ok = fn(t, needle);
    std::cout << (ok ? "true" : "false") << '\n';
    free_tree(t);
    return 0;
}

// two trees -> bool
template <class F>
int run_tree_eq(F fn) {
    Node *a = read_tree();
    Node *b = read_tree();
    bool ok = fn(a, b);
    std::cout << (ok ? "true" : "false") << '\n';
    free_tree(a);
    free_tree(b);
    return 0;
}

// tree -> tree
template <class F>
int run_tree_map(F fn) {
    Node *t = read_tree();
    Node *out = fn(t);
    write_tree(out);
    // out may alias t (in-place invert) — free once
    free_tree(out);
    return 0;
}

// adj list + source + sink -> optional path
template <class F>
int run_graph_path(F fn) {
    auto g = read_adj_list();
    int source = 0, sink = 0;
    std::cin >> source >> sink;
    write_optional_list(fn(g, source, sink));
    return 0;
}

// matrix + source + sink -> optional path
template <class F>
int run_matrix_path(F fn) {
    auto g = read_matrix();
    int source = 0, sink = 0;
    std::cin >> source >> sink;
    write_optional_list(fn(g, source, sink));
    return 0;
}

// adj list -> optional adj list (MST)
template <class F>
int run_graph_mst(F fn) {
    auto g = read_adj_list();
    auto out = fn(g);
    if (!out) {
        std::cout << "null\n";
    } else {
        write_adj_list(*out);
    }
    return 0;
}

// n + edge pairs -> list (topo)
template <class F>
int run_topo(F fn) {
    int n = 0;
    std::cin >> n;
    auto edges = read_pairs();
    write_list_i64(fn(n, edges));
    return 0;
}

// list -> list of lists (subsets/perms)
template <class F>
int run_list_lists(F fn) {
    auto a = read_list_i64();
    write_list_of_lists(fn(a));
    return 0;
}

// n k -> list of lists
template <class F>
int run_combos(F fn) {
    std::int64_t n = 0, k = 0;
    std::cin >> n >> k;
    write_list_of_lists(fn(n, k));
    return 0;
}

// maze
template <class F>
int run_maze(F fn) {
    int rows = 0, cols = 0;
    std::cin >> rows >> cols;
    std::cin.get();
    std::vector<std::string> maze(static_cast<std::size_t>(rows));
    for (int i = 0; i < rows; ++i) {
        std::getline(std::cin, maze[static_cast<std::size_t>(i)]);
        if (static_cast<int>(maze[static_cast<std::size_t>(i)].size()) > cols) {
            maze[static_cast<std::size_t>(i)].resize(static_cast<std::size_t>(cols));
        }
    }
    std::string wall;
    std::cin >> wall;
    Point start{}, end{};
    std::cin >> start.x >> start.y >> end.x >> end.y;
    auto path = fn(maze, wall, start, end);
    std::cout << path.size() << '\n';
    for (const auto &p : path) {
        std::cout << p.x << ' ' << p.y << '\n';
    }
    return 0;
}

// linked list cycle
template <class F>
int run_cycle(F fn) {
    int cycle_to = -1;
    ListNode *head = read_list_nodes(cycle_to);
    bool ok = fn(head);
    std::cout << (ok ? "true" : "false") << '\n';
    free_list_nodes(head, cycle_to >= 0);
    return 0;
}

} // namespace kata
