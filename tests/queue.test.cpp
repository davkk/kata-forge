#include "kata_test.h"
#include <optional>

int main() { TEST("basic_fifo");
    Queue _inst0;
    _inst0.push(5);
    _inst0.push(7);
    _inst0.push(9);
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 5); }
    CHECK(_inst0.size() == 2);
    _inst0.push(11);
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 7); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.front(); CHECK(_v.has_value() && *_v == 11); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 11); }
    { auto _v = _inst0.pop(); CHECK(!_v.has_value()); }
    CHECK(_inst0.size() == 0);
    _inst0.push(69);
    { auto _v = _inst0.front(); CHECK(_v.has_value() && *_v == 69); }
    CHECK(_inst0.size() == 1);

    // empty_deque
    Queue _inst1;
    { auto _v = _inst1.pop(); CHECK(!_v.has_value()); }

    // empty_peek
    Queue _inst2;
    { auto _v = _inst2.front(); CHECK(!_v.has_value()); }

    return 0;
}
