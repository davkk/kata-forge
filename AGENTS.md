# kata-forge — agent notes

C++ DSA practice forge. Compile-time assertions replace wire formats and I/O piping.

## Core model

- **Catalog** `catalog/<id>.json` — one file per kata: meta, description, interface, named tests (`in`/`out`).
- **Sessions** live in-repo: `sessions/sessionN/`. Pick list: `.active.json`.
- **Goldens** `goldens/<id>.md` — teaching tutorials (intuition, code, alternatives, where it shows up, cousins & contrasts).
- **CLIs** (`cli/`, Python 3 **stdlib only**, entrypoint `./kf`): `list`, `pick`, `generate`, `test`, `grade`, `reset`.

## How testing works

- Each `.cpp` in a session defines types + a `solve` (or named) function. No `main()`, no framework header.
- `./kf test` generates a C++ harness that `#include`s the solution file, builds test inputs from catalog JSON as C++ objects, calls the function, and `assert`s results.
- Compilation catches type errors; assertion failures identify wrong answers. No I/O piping, no wire format.
- Command katas (stack, heap, LRU, …) are tested by instantiating a class and calling methods directly.

## Stubs

- `./kf generate` writes `sessions/sessionN/<id>.cpp`.
- Every stub includes `<bits/stdc++.h>` + `using namespace std;` + the type definitions and function signature.
- Problem text in a short `/* ... */` block: title, description, interface.
- User fills in the function body.

## Goldens (markdown)

- Teaching tutorials: intuition → main approach + code → alternatives → where it shows up → cousins & contrasts.
- Code in **C-style C++** with `using namespace std;` — raw pointers for linked structures, plain arrays/vectors, **`struct` with members + methods** for command-shape katas (matches the stub generator), free functions for function-shape katas. No templates, no access specifiers (struct members are public by default).
- Method/function names and parameter names in the golden code MUST match the catalog's `cpp.methods` / `cpp.fn` exactly. The golden is what the user sees alongside their stub.
- **Do not define auxiliary struct types** (Node, Edge, KV, …) in the golden code. If the catalog has them in `cpp.members` or `cpp.structs`, the user already has them from the stub. Refer to them by name; let the user figure out the fields if not in the catalog.
- Bullet points only; **no tables**, no numbered step-lists that merely paraphrase code.
- ~50–100 lines per file (≤120 for range queries; ≥40 for trivial structures). Every line justifies itself.
- No session or test infrastructure mentioned — pure DSA teaching.

## Adding a kata

1. `catalog/<id>.json` with `shape` + named tests. Shape determines the calling convention (function signature + how the harness builds inputs).
2. `goldens/<id>.md` reference write-up.
3. If using a new shape, add its function signature + input builder + assertion logic in `kflib/kf_common.py` (shape sigs, input builders).

## Do not

- Add pip dependencies.
- Make stub comments or goldens verbose.
- **Never** remove or modify user sessions (`sessions/sessionN/`). Session data is untracked by git and irrecoverable.
