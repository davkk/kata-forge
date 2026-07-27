#include "kata_test.h"
#include <optional>
#include <string>
#include <vector>

int main() { TEST("insert_find_remove_prefix");
    Trie _inst0;
    _inst0.insert("hello");
    _inst0.insert("help");
    _inst0.insert("world");
    _inst0.insert("word");
    vector<string> _exp1 = {"hello", "help"};
    auto _r1 = _inst0.find("hel");
    CHECK(_r1.size() == _exp1.size());
    for (size_t i = 0; i < _r1.size(); ++i)
        CHECK(_r1[i] == _exp1[i]);
    _inst0.remove("help");
    vector<string> _exp2 = {"hello"};
    auto _r2 = _inst0.find("hel");
    CHECK(_r2.size() == _exp2.size());
    for (size_t i = 0; i < _r2.size(); ++i)
        CHECK(_r2[i] == _exp2[i]);
    vector<string> _r3 = _inst0.find("xyz");
    CHECK(_r3.empty());

    return 0;
}
