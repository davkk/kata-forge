#!/usr/bin/env python3
"""Shared helpers for kata-forge CLIs. C++ only."""

from __future__ import annotations

import json
from pathlib import Path

TOOL_ROOT = Path(__file__).resolve().parent.parent
CATALOG_DIR = TOOL_ROOT / "catalog"
TESTS_DIR = TOOL_ROOT / "tests"


def sessions_dir() -> Path:
    return Path.cwd() / "sessions"


def active_path() -> Path:
    return Path.cwd() / ".active.json"


def ensure_sessions_dir() -> None:
    sessions_dir().mkdir(parents=True, exist_ok=True)


def load_catalog(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def list_katas() -> list[dict]:
    katas = []
    for path in sorted(CATALOG_DIR.glob("*.json")):
        data = load_catalog(path)
        if not data or "id" not in data:
            continue
        data["_path"] = path
        katas.append(data)
    return katas


def kata_by_id(kid: str) -> dict | None:
    path = CATALOG_DIR / f"{kid}.json"
    if not path.exists():
        return None
    data = load_catalog(path)
    data["_path"] = path
    return data


def read_json(path: Path, fallback):
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return fallback


def write_json(path: Path, data) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")


def latest_session() -> Path | None:
    d = sessions_dir()
    if not d.exists():
        return None
    sessions = [p for p in d.iterdir() if p.is_dir()]
    if not sessions:
        return None
    return max(sessions, key=lambda p: p.name)


def new_session_id() -> str:
    ensure_sessions_dir()
    sd = sessions_dir()
    nums = []
    for p in sd.iterdir():
        if p.is_dir() and p.name.startswith("session"):
            suffix = p.name[len("session") :]
            if suffix.isdigit():
                nums.append(int(suffix))
    n = (max(nums) + 1) if nums else 1
    return f"session{n}"


# --- comment block ------------------------------------------------------------

def kata_comment(kata: dict) -> str:
    lines = ["/*", f" * {kata.get('title', kata.get('id', ''))}", " *"]
    desc = (kata.get("description") or "").rstrip()
    if desc:
        for line in desc.splitlines():
            lines.append(f" * {line}" if line.strip() else " *")
        lines.append(" *")
    iface = (kata.get("interface") or "").rstrip()
    if iface:
        lines.append(" * interface:")
        for part in iface.splitlines():
            stripped = part.strip()
            lines.append(f" *   {stripped}" if stripped else " *")
    lines.append(" */")
    return "\n".join(lines)


# --- stub generation ----------------------------------------------------------

def _split_fn(s: str) -> tuple[str, str, str]:
    """Parse 'ret name(params)' into (ret, name, params)."""
    paren = s.index("(")
    before = s[:paren].rsplit(None, 1)
    ret = before[0] if len(before) > 1 else ""
    name = before[-1]
    params = s[paren+1:s.index(")")]
    return ret, name, params


def render_stub(kata: dict) -> str:
    cpp = kata.get("cpp", {})
    kid = kata["id"]
    shape = kata.get("shape", "")

    parts = [kata_comment(kata)]
    parts.append("#include <bits/stdc++.h>")
    parts.append("using namespace std;")
    parts.append("")

    if shape == "commands":
        cls = cpp["class"]
        parts.append(f"struct {cls} {{")
        for m in cpp.get("members", []):
            parts.append(f"    {m};")
        ctor_param = cpp.get("ctor", "")
        if cpp.get("members") or ctor_param:
            parts.append("")
        if ctor_param:
            parts.append(f"    {cls}({ctor_param}) {{")
            parts.append("        // TODO")
            parts.append("    }")
            parts.append("")
        for m in cpp.get("methods", []):
            ret, name, params = _split_fn(m)
            sig = f"{params}" if params else ""
            parts.append(f"    {ret} {name}({sig}) {{")
            parts.append("        // TODO")
            if ret.startswith("optional"):
                parts.append("        return nullopt;")
            elif ret == "bool":
                parts.append("        return false;")
            elif ret in ("int", "int64_t"):
                parts.append("        return 0;")
            parts.append("    }")
            parts.append("")
        parts.append("};")
    else:
        for s in cpp.get("structs", []):
            parts.append(s)
            parts.append("")
        fn = cpp.get("fn", "")
        ret, name, params = _split_fn(fn)
        if "hint" in cpp:
            parts.append(cpp["hint"])
        parts.append(f"{ret} {name}({params}) {{")
        parts.append("    // TODO")
        parts.append("}")

    return "\n".join(parts) + "\n"
