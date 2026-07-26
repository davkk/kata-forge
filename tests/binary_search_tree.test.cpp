#include "kata_test.h"
#include <optional>

int main() { TEST("insert_find_delete");
    BinarySearchTree _inst0;
    CHECK(_inst0.size() == 0);
    CHECK(!_inst0.find(50).has_value());
    _inst0.insert(50);
    CHECK(_inst0.size() == 1);
    { auto _v = _inst0.find(50); CHECK(_v.has_value() && *_v == 50); }
    _inst0.insert(30);
    _inst0.insert(80);
    _inst0.insert(20);
    _inst0.insert(40);
    _inst0.insert(70);
    _inst0.insert(90);
    CHECK(_inst0.size() == 7);
    { auto _v = _inst0.find(20); CHECK(_v.has_value() && *_v == 20); }
    { auto _v = _inst0.find(90); CHECK(_v.has_value() && *_v == 90); }
    CHECK(!_inst0.find(100).has_value());
    _inst0.remove(20);
    CHECK(_inst0.size() == 6);
    CHECK(!_inst0.find(20).has_value());
    _inst0.remove(30);
    CHECK(_inst0.size() == 5);
    _inst0.remove(80);
    CHECK(_inst0.size() == 4);
    { auto _v = _inst0.find(40); CHECK(_v.has_value() && *_v == 40); }
    _inst0.remove(50);
    _inst0.remove(70);
    CHECK(_inst0.size() == 2);

    return 0;
}
