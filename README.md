# kata-forge

Multi-language DSA practice. Catalog is language-neutral; v1 ships a **C++** helper lib.

### Setup

Needs `python3` (stdlib only — no pip packages) and a C++17 compiler (`c++` / `g++` / `clang++`).

### Workflow

```bash
./pick --katas queue,stack   # or: ./pick --count 5
./generate                         # → sessions/sessionN/<id>.cpp stubs
# edit sessions/sessionN/<id>.cpp
./test
```

Everything stays in the repo: `sessions/sessionN/`, `.active.json`.  
`./generate` writes C++ files with a short `/* ... */` problem block plus empty `solve` + `main` wired to the shape helper.  
Catalog has 50+ katas ported from kata-machine (sorts, lists, trees, graphs, DP, …).

### Commands

| Command | Description |
|---------|-------------|
| `./list` | List catalog katas |
| `./pick [--count N]` | Random pick → `.active.json` |
| `./pick --katas a,b` | Explicit pick |
| `./generate` | New `sessions/sessionN/` with commented `.cpp` stubs |
| `./test` | Compile `*.cpp` in latest session and run shape I/O tests |
| `./test --kata ID --cmd '...'` | Judge any command that speaks the shape wire format |

### C++ solutions

```cpp
#include "kata.hpp"

static void solve(std::vector<std::int64_t>& a) {
    // ...
}

int main() {
    return kata::run_list_inplace(solve);
}
```

`./test` compiles with `-I lib/cpp`. See `shapes.md` and `lib/cpp/kata.hpp` for every shape’s `run_*` helper.

### Layout

```
catalog/          one JSON file per kata (source of truth)
lib/cpp/kata.hpp  shape I/O helpers
goldens/*.md      reference write-ups (code structure + use cases)
sessions/         your practice sessions
.active.json      current pick list
shapes.md         wire formats
```

### Adding a language later

1. Add `lib/<lang>/` with the same helpers (`run_list_inplace`, `run_commands`, …)
2. Teach `./test` how to invoke that language’s files
3. Catalog and wire formats stay unchanged
