#include "kata_test.h"
#include <optional>

int main() { TEST("insert_and_extract_min_in_order");
    MinHeap _inst0;
    EQL(0, _inst0.length());
    _inst0.insert(5);
    _inst0.insert(3);
    _inst0.insert(69);
    _inst0.insert(420);
    _inst0.insert(4);
    _inst0.insert(1);
    _inst0.insert(8);
    _inst0.insert(7);
    EQL(8, _inst0.length());
    { auto _v = _inst0.deleteMin(); VAL(_v, 1); }
    { auto _v = _inst0.deleteMin(); VAL(_v, 3); }
    { auto _v = _inst0.deleteMin(); VAL(_v, 4); }
    { auto _v = _inst0.deleteMin(); VAL(_v, 5); }
    EQL(4, _inst0.length());
    { auto _v = _inst0.deleteMin(); VAL(_v, 7); }
    { auto _v = _inst0.deleteMin(); VAL(_v, 8); }
    { auto _v = _inst0.deleteMin(); VAL(_v, 69); }
    { auto _v = _inst0.deleteMin(); VAL(_v, 420); }
    EQL(0, _inst0.length());

    return 0;
}
