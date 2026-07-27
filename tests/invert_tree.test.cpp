#include "kata_test.h"

static bool eq_tree(Node* a, Node* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return a->val == b->val && eq_tree(a->left, b->left) && eq_tree(a->right, b->right);
}

int main() {
    { TEST("non_empty_tree");
        Node* _tree = new Node{4, new Node{2, new Node{1, nullptr, nullptr}, new Node{3, nullptr, nullptr}}, new Node{7, new Node{6, nullptr, nullptr}, new Node{9, nullptr, nullptr}}};
        Node* _exp  = new Node{4, new Node{7, new Node{9, nullptr, nullptr}, new Node{6, nullptr, nullptr}}, new Node{2, new Node{3, nullptr, nullptr}, new Node{1, nullptr, nullptr}}};
        auto _r = invert(_tree);
        CHECK(eq_tree(_r, _exp));
    }

    { TEST("null_tree");
        Node* _tree = nullptr;
        auto _r = invert(_tree);
        EQL(nullptr, _r);
    }

    return 0;
}
