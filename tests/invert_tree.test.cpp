#include "kata_test.h"

int main() {
    { TEST("basic");
        Node* _tree = new Node{4, new Node{2, new Node{1, nullptr, nullptr}, new Node{3, nullptr, nullptr}}, new Node{7, new Node{6, nullptr, nullptr}, new Node{9, nullptr, nullptr}}};
        auto _r = invert(_tree);
        CHECK(_r != nullptr); // TODO: deep tree compare
    }

    { TEST("null");
        Node* _tree = nullptr;
        auto _r = invert(_tree);
        CHECK(_r == nullptr);
    }

    return 0;
}
