#include "kata_test.h"
#include <optional>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    { TEST("insert_find_remove_prefix");
        Trie _inst0;
        _inst0.insert("hello");
        _inst0.insert("help");
        _inst0.insert("world");
        _inst0.insert("word");
        vector<string> _exp1 = {"hello", "help"};
        auto _r1 = _inst0.find("hel");
        sort(_r1.begin(), _r1.end());
        EQL(_exp1, _r1);
        _inst0.remove("help");
        vector<string> _exp2 = {"hello"};
        auto _r2 = _inst0.find("hel");
        EQL(_exp2, _r2);
        vector<string> _r3 = _inst0.find("xyz");
        CHECK(_r3.empty());
    }

    return 0;
}
