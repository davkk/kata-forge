#include "kata_test.h"
#include <optional>

int main() { TEST("basic_lifo");
    Stack _inst0;
    _inst0.push(5);
    _inst0.push(7);
    _inst0.push(9);
    { auto _v = _inst0.pop(); VAL(_v, 9); }
    EQL(2, _inst0.size());
    _inst0.push(11);
    { auto _v = _inst0.pop(); VAL(_v, 11); }
    { auto _v = _inst0.pop(); VAL(_v, 7); }
    { auto _v = _inst0.peek(); VAL(_v, 5); }
    { auto _v = _inst0.pop(); VAL(_v, 5); }
    { auto _v = _inst0.pop(); CHECK(!_v.has_value()); }
    _inst0.push(69);
    { auto _v = _inst0.peek(); VAL(_v, 69); }
    EQL(1, _inst0.size());

    return 0;
}
