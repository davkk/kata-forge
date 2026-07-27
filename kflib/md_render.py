#!/usr/bin/env python3
"""Minimal stdlib markdown → ANSI renderer.

Handles the subset used by goldens/: ATX headers, fenced code blocks, bullet
lists, and inline `**bold**`, `*italic*`, `` `code` ``. No tables, links, or
nested lists. ~90 lines, no deps.
"""

from __future__ import annotations

import os
import re
import sys


BOLD = "\x1b[1m"
ITALIC = "\x1b[3m"
DIM = "\x1b[2m"
CYAN = "\x1b[36m"
YELLOW = "\x1b[33m"
RESET = "\x1b[0m"


def wants_color(stream=sys.stdout) -> bool:
    if os.environ.get("NO_COLOR") is not None:
        return False
    return bool(getattr(stream, "isatty", lambda: False)())


_INLINE_RE = re.compile(
    r"\*\*([^*\n]+)\*\*"
    r"|(?<![*\w])\*([^*\n]+)\*(?!\w)"
    r"|`([^`\n]+)`"
)


def _inline_color(line: str) -> str:
    out: list[str] = []
    pos = 0
    for m in _INLINE_RE.finditer(line):
        if m.start() > pos:
            out.append(line[pos:m.start()])
        if m.group(1) is not None:
            out.append(f"{BOLD}{_inline_color(m.group(1))}{RESET}")
        elif m.group(2) is not None:
            out.append(f"{ITALIC}{_inline_color(m.group(2))}{RESET}")
        else:
            out.append(f"{DIM}{m.group(3)}{RESET}")
        pos = m.end()
    if pos < len(line):
        out.append(line[pos:])
    return "".join(out)


def _inline_plain(line: str) -> str:
    out: list[str] = []
    pos = 0
    for m in _INLINE_RE.finditer(line):
        if m.start() > pos:
            out.append(line[pos:m.start()])
        out.append(m.group(1) or m.group(2) or m.group(3))
        pos = m.end()
    if pos < len(line):
        out.append(line[pos:])
    return "".join(out)


_CODE_FENCE = re.compile(r"^```")


def render(md: str, color: bool) -> str:
    inline = _inline_color if color else _inline_plain
    lines = md.splitlines()
    out: list[str] = []
    in_code = False
    code: list[str] = []

    def flush_code() -> None:
        if not code:
            return
        if color:
            out.append(f"{DIM}  {'─' * 60}{RESET}")
            for cl in code:
                out.append(f"{DIM}  {cl}{RESET}")
            out.append(f"{DIM}  {'─' * 60}{RESET}")
        else:
            out.extend(f"  {cl}" for cl in code)
        code.clear()

    for line in lines:
        if _CODE_FENCE.match(line.strip()):
            if in_code:
                flush_code()
                in_code = False
            else:
                in_code = True
            continue
        if in_code:
            code.append(line)
            continue

        m = re.match(r"^(#{1,6})\s+(.*)$", line)
        if m:
            level, text = len(m.group(1)), m.group(2)
            rendered = inline(text)
            if color:
                if level == 1:
                    out.append(f"{BOLD}{CYAN}{rendered}{RESET}")
                elif level == 2:
                    out.append(f"{BOLD}{CYAN}{rendered}{RESET}")
                else:
                    out.append(f"{CYAN}{rendered}{RESET}")
            else:
                out.append(rendered)
            continue

        m = re.match(r"^(\s*)-\s+(.*)$", line)
        if m:
            indent, text = m.group(1), m.group(2)
            bullet = f"{YELLOW}•{RESET}" if color else "-"
            out.append(f"{indent}{bullet} {inline(text)}")
            continue

        if not line.strip():
            out.append("")
            continue

        out.append(inline(line))

    if in_code:
        flush_code()

    return "\n".join(out) + "\n"
