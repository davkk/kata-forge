#include "kata_test.h"
#include <optional>

int main() {
    { TEST("basic_fifo");
        Queue _inst0;
        _inst0.enqueue(5);
        _inst0.enqueue(7);
        _inst0.enqueue(9);
        { auto _v = _inst0.deque(); VAL(_v, 5); }
        EQL(2, _inst0.size());
        _inst0.enqueue(11);
        { auto _v = _inst0.deque(); VAL(_v, 7); }
        { auto _v = _inst0.deque(); VAL(_v, 9); }
        { auto _v = _inst0.peek(); VAL(_v, 11); }
        { auto _v = _inst0.deque(); VAL(_v, 11); }
        { auto _v = _inst0.deque(); CHECK(!_v.has_value()); }
        EQL(0, _inst0.size());
        _inst0.enqueue(69);
        { auto _v = _inst0.peek(); VAL(_v, 69); }
        EQL(1, _inst0.size());
    }

    { TEST("empty_dequeue");
        Queue _inst1;
        { auto _v = _inst1.deque(); CHECK(!_v.has_value()); }
    }

    { TEST("empty_peek");
        Queue _inst2;
        { auto _v = _inst2.peek(); CHECK(!_v.has_value()); }
    }

    return 0;
}
