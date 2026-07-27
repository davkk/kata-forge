#include "kata_test.h"
#include <optional>

int main() {
    { TEST("push_pop_get");
        auto _inst0 = RingBuffer{3};
        _inst0.push(5);
        { auto _v = _inst0.pop(); VAL(_v, 5); }
        { auto _v = _inst0.pop(); CHECK(!_v.has_value()); }
        _inst0.push(42);
        _inst0.push(9);
        { auto _v = _inst0.pop(); VAL(_v, 42); }
        { auto _v = _inst0.pop(); VAL(_v, 9); }
        { auto _v = _inst0.pop(); CHECK(!_v.has_value()); }
        _inst0.push(42);
        _inst0.push(9);
        _inst0.push(12);
        { auto _v = _inst0.get(2); VAL(_v, 12); }
        { auto _v = _inst0.get(1); VAL(_v, 9); }
        { auto _v = _inst0.get(0); VAL(_v, 42); }
    }

    { TEST("wraparound");
        auto _inst1 = RingBuffer{3};
        _inst1.push(1);
        _inst1.push(2);
        _inst1.push(3);
        { auto _v = _inst1.pop(); VAL(_v, 1); }
        { auto _v = _inst1.pop(); VAL(_v, 2); }
        _inst1.push(4);
        _inst1.push(5);
        { auto _v = _inst1.get(0); VAL(_v, 3); }
        { auto _v = _inst1.get(1); VAL(_v, 4); }
        { auto _v = _inst1.get(2); VAL(_v, 5); }
        { auto _v = _inst1.pop(); VAL(_v, 3); }
        { auto _v = _inst1.pop(); VAL(_v, 4); }
        { auto _v = _inst1.pop(); VAL(_v, 5); }
        { auto _v = _inst1.pop(); CHECK(!_v.has_value()); }
    }

    return 0;
}
