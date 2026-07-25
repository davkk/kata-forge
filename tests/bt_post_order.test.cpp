#include <cassert>

int main() {
    { // tree
        Node* _tree = new Node{20, new Node{10, new Node{5, nullptr, new Node{7, nullptr, nullptr}}, new Node{15, nullptr, nullptr}}, new Node{50, new Node{30, new Node{29, nullptr, nullptr}, new Node{45, nullptr, nullptr}}, new Node{100, nullptr, nullptr}}};
        auto _r = pre_order(_tree);
        { auto _exp = vector<int>{7, 5, 15, 10, 29, 45, 30, 100, 50, 20}; assert(_r == _exp); }
    }

    return 0;
}
