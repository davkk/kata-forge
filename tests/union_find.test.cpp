#include <cassert>
#include <optional>

int main() {
    // basic
    auto _inst0 = Impl{{10}};
    // unknown op: length
    // unknown op: connected
    // unknown op: union
    // unknown op: connected
    // unknown op: union
    // unknown op: union
    // unknown op: union
    // unknown op: connected
    // unknown op: connected
    // unknown op: union
    // unknown op: connected
    // unknown op: connected

    // find_same_component
    auto _inst1 = Impl{{10}};
    // unknown op: union
    // unknown op: union
    // unknown op: union
    // unknown op: union
    // unknown op: union
    // unknown op: connected
    // unknown op: connected

    return 0;
}
