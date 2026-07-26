#include "kata_test.h"

int main() {
    { TEST("unique_path");
        vector<string> _maze = {"xxxxxxxxxx x", "x        x x", "x        x x", "x xxxxxxxx x", "x          x", "x xxxxxxxxxx"};
        string _wall = "x";
        Point _start = {10, 0};
        Point _end = {1, 5};
        auto _r = solve_maze(_maze, _wall, _start, _end);
        CHECK(_r == vector<Point>{{10,0}, {10,1}, {10,2}, {10,3}, {10,4}, {9,4}, {8,4}, {7,4}, {6,4}, {5,4}, {4,4}, {3,4}, {2,4}, {1,4}, {1,5}});
    }

    return 0;
}
