# kata-forge -- agent notes

C++ DSA practice forge. Compile-time assertions replace wire formats and I/O piping.

## Core model

- **Catalog** `catalog/<id>.json` is the single source of truth for interfaces: meta + `cpp` contract. Stubs are generated from it; goldens and tests must match it.
- **Tests** `tests/<id>.test.cpp`: handwritten C++ harnesses using `include/kata_test.h` (`TEST`/`EQL`/`VAL`/`CHECK`).
- **Goldens** `goldens/<id>.md`: teaching tutorials. **Sessions** `sessions/sessionN/` (pick list `.active.json`, FSRS state `.fsrs.json`).
- **CLIs** (`cli/`, Python 3 stdlib only, entrypoint `./kf`): list, pick, generate, test, grade, golden.

## How testing works

- `./kf test` compiles `tests/<id>.test.cpp` with the solution force-included (`-include solution.cpp`); compile errors catch type mismatches, `EQL`/`VAL`/`CHECK` failures catch wrong answers. No I/O piping.

## Stubs

- `./kf generate` writes `sessions/sessionN/<id>.cpp`: `<bits/stdc++.h>` + `using namespace std;` + types + signature + short `/* ... */` problem block. User fills the body.

## Goldens

- **Teach, don't show**: lead with the core principle (problem-agnostic invariant/mechanism), then build the solution in a `## Build up` of tiny chained code snippets (plain ``` blocks) -- naive -> limit -> insight -> final code.
- **Layout**: `# Title` (essence) -> `## Core idea` (invariant + why correct, no triggers) -> `## Build up` (snippet story) -> ASCII diagram (plain ```, NOT ```cpp) -> `## Approach` (verified code, `// step N` marks) -> `### Trace` -> `## Complexity` -> `## Alternative` -> `## Use when` (triggers live here) -> `## Cousins`.
- **Scannable**: ~45-70 lines; short bullets; no filler, no tables, no em/en dashes, no numbered lists that paraphrase code.
- The first ```cpp block must be complete and self-contained and pass `tests/<id>.test.cpp`. No auxiliary structs (Node/Edge/KV) -- the catalog provides them. Method/param names must match the catalog exactly.
- Containers are `class X { public: ... }` with a constructor that initializes the members (default or with args); auxiliary data types (Node/Edge/KV/Point/ListNode) stay `struct`.

## Conventions

- Count: `size()`. Remove-by-value: `remove(x)`. End-pops: stack `pop`, queue `deque`, min_heap `deleteMin`, ring_buffer `pop`.
- Containers: `class X { public: ... }`, every one has a ctor (`X() : field(default), ... {}` or `X(args) : ...`). Aux data types stay `struct`.
- Pointer style `Type* name`; members end with `;`. `cpp.members` = complete state; `cpp.ctor` when construction takes args (empty string = default ctor).
- `cpp.hint`: one terse `//` line stating the return + sentinels. Description: one lowercase sentence; interface: one signature per line with `-> ret`.

## Adding a kata

1. `catalog/<id>.json` (commands: `class` + `methods`, optional `members`/`ctor`; else `fn`, optional `structs`/`hint`).
2. `goldens/<id>.md`; 3. `tests/<id>.test.cpp`.
- No per-kata headers (`include/<id>.h`); only touch `kflib/kf_common.py` for a new stub kind.

## Working style

- Use subagents (task tool) for large/batch work; keep shared files (kflib, docs, conventions) for the main agent.

## Do not

- Add pip deps; make stubs/goldens verbose; touch `sessions/` (untracked, irrecoverable).
