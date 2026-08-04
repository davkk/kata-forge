#include "kata_test.h"
#include <optional>

int main() { TEST("insert_find_remove_size");
    BinarySearchTree _inst0;
    EQL(0, _inst0.size());
    CHECK(!_inst0.find(50).has_value());
    _inst0.insert(50);
    EQL(1, _inst0.size());
    { auto _v = _inst0.find(50); VAL(_v, 50); }
    _inst0.insert(30);
    _inst0.insert(80);
    _inst0.insert(20);
    _inst0.insert(40);
    _inst0.insert(70);
    _inst0.insert(90);
    EQL(7, _inst0.size());
    { auto _v = _inst0.find(20); VAL(_v, 20); }
    { auto _v = _inst0.find(90); VAL(_v, 90); }
    CHECK(!_inst0.find(100).has_value());
    _inst0.remove(20);
    EQL(6, _inst0.size());
    CHECK(!_inst0.find(20).has_value());
    _inst0.remove(30);
    EQL(5, _inst0.size());
    _inst0.remove(80);
    EQL(4, _inst0.size());
    { auto _v = _inst0.find(40); VAL(_v, 40); }
    _inst0.remove(50);
    _inst0.remove(70);
    EQL(2, _inst0.size());

    return 0;
}
