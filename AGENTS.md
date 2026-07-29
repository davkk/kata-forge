# kata-forge — agent notes

C++ DSA practice forge. Compile-time assertions replace wire formats and I/O piping.

## Core model

- **Catalog** `catalog/<id>.json` — one file per kata: meta (title, shape, description, interface, example) + the `cpp` interface contract. **The catalog is the single source of truth for interfaces** — stubs are generated from it, goldens and tests must match it.
- **Tests** `tests/<id>.test.cpp` — handwritten C++ harnesses using the mini-framework in `include/kata_test.h` (`TEST`/`EQL`/`VAL`/`CHECK` macros, pretty-printed diffs).
- **Sessions** live in-repo: `sessions/sessionN/`. Pick list: `.active.json`. FSRS state: `.fsrs.json`.
- **Goldens** `goldens/<id>.md` — teaching tutorials (intuition, code, alternatives, where it shows up, cousins & contrasts).
- **CLIs** (`cli/`, Python 3 **stdlib only**, entrypoint `./kf`): `list`, `pick`, `generate`, `test`, `grade`, `golden`.

## How testing works

- Each `.cpp` in a session defines types + a `solve` (or named) function. No `main()`, no framework header.
- `./kf test` compiles the handwritten harness `tests/<id>.test.cpp` with the solution force-included: `c++ -std=c++17 -O2 -Wall -include solution.cpp -Iinclude tests/<id>.test.cpp`.
- Compilation catches type errors; `EQL`/`VAL`/`CHECK` assertion failures identify wrong answers with a pretty-printed diff. No I/O piping, no wire format.
- Command katas (stack, heap, LRU, …) are tested by instantiating the catalog-declared struct and calling methods directly.

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
- The **first ` ```cpp ` block (main approach) must be complete and self-contained** given catalog-provided types: it is verified by compiling and running it against `tests/<id>.test.cpp`. No helpers stranded in later blocks.
- Bullet points only; **no tables**, no numbered step-lists that merely paraphrase code.
- ~50–100 lines per file (≤120 for range queries; ≥40 for trivial structures). Every line justifies itself.
- No session or test infrastructure mentioned — pure DSA teaching.

## Adding a kata

1. `catalog/<id>.json` — meta + `cpp` block. `shape: "commands"` needs `class` + `methods` (optional `members`, `ctor`); any other shape needs `fn` (optional `structs`, `hint`).
2. `goldens/<id>.md` — reference write-up.
3. `tests/<id>.test.cpp` — handwritten harness: `#include "kata_test.h"`, one `TEST("name")` per case, `EQL`/`VAL`/`CHECK` assertions.
- Only touch `kflib/kf_common.py` if a new stub *kind* is needed — `render_stub` already handles commands vs function shapes generically.
- Do **not** create `include/<id>.h` — per-kata headers were removed as dead duplicates; the catalog is the only interface declaration.

## Working style

- Use subagents (the `task` tool) for large work: whole-project audits, batch fixes, multi-file reviews. Delegate per-kata/per-file work in parallel batches; keep shared files (kflib, docs, catalog conventions) for the main agent.

## Do not

- Add pip dependencies.
- Make stub comments or goldens verbose.
- **Never** remove or modify user sessions (`sessions/sessionN/`). Session data is untracked by git and irrecoverable.
