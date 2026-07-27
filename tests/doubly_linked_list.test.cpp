#include "kata_test.h"
#include <optional>

int main() { TEST("append_remove_prepend_get_size");
    DoublyLinkedList _inst0;
    _inst0.append(5);
    _inst0.append(7);
    _inst0.append(9);
    { auto _v = _inst0.get(2); CHECK(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.removeAt(1); CHECK(_v.has_value() && *_v == 7); }
    CHECK(_inst0.size() == 2);
    _inst0.append(11);
    { auto _v = _inst0.removeAt(1); CHECK(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.remove(9); CHECK(!_v.has_value()); }
    { auto _v = _inst0.removeAt(0); CHECK(_v.has_value() && *_v == 5); }
    { auto _v = _inst0.removeAt(0); CHECK(_v.has_value() && *_v == 11); }
    CHECK(_inst0.size() == 0);
    _inst0.prepend(5);
    _inst0.prepend(7);
    _inst0.prepend(9);
    { auto _v = _inst0.get(2); CHECK(_v.has_value() && *_v == 5); }
    { auto _v = _inst0.get(0); CHECK(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.remove(9); CHECK(_v.has_value() && *_v == 9); }
    CHECK(_inst0.size() == 2);
    { auto _v = _inst0.get(0); CHECK(_v.has_value() && *_v == 7); }

    return 0;
}
