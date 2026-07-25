#include <cassert>
#include <optional>

int main() {
    // list_ops
    SinglyLinkedList _inst0;
    _inst0.append(5);
    _inst0.append(7);
    _inst0.append(9);
    { auto _v = _inst0.get(2); assert(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.removeAt(1); assert(_v.has_value() && *_v == 7); }
    assert(_inst0.size() == 2);
    _inst0.append(11);
    { auto _v = _inst0.removeAt(1); assert(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.remove(9); assert(!_v.has_value()); }
    { auto _v = _inst0.removeAt(0); assert(_v.has_value() && *_v == 5); }
    { auto _v = _inst0.removeAt(0); assert(_v.has_value() && *_v == 11); }
    assert(_inst0.size() == 0);
    _inst0.prepend(5);
    _inst0.prepend(7);
    _inst0.prepend(9);
    { auto _v = _inst0.get(2); assert(_v.has_value() && *_v == 5); }
    { auto _v = _inst0.get(0); assert(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.remove(9); assert(_v.has_value() && *_v == 9); }
    assert(_inst0.size() == 2);
    { auto _v = _inst0.get(0); assert(_v.has_value() && *_v == 7); }

    return 0;
}
