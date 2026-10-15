"""Runs packages/'s Python from the old tools/ paths, with nothing installed (#1167).

The Python tools are packages/buildtools (the commands) and
packages/crashdata (the games' data); tools/<name>.py is a shim for each.
Run, it runs the command's main() (`python3 tools/levels.py ...`, as the
Makefile, CI and the docs do); imported (with tools/ on sys.path,
`import levels`), it is the module its code moved to. Importing this puts
the packages' src/ directories on sys.path, so no `uv sync` or pip install
is needed (with the workspace installed, the commands are also console
scripts, crash-<name>: see pyproject.toml).
"""
import importlib
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
for _package in ("crashdata", "buildtools"):
    _src = os.path.join(ROOT, "packages", _package, "src")
    if _src not in sys.path:
        sys.path.insert(1, _src)


def shim(name, command, module=None, exit_status=True):
    """The shim tools/<x>.py, whose __name__ is `name`: run, `command`'s
    main() (its return value the exit status, unless not `exit_status`);
    imported, the module `module` (`command` by default) in its place."""
    if name == "__main__":
        main = getattr(importlib.import_module(command), "main", None)
        if main is not None:
            status = main()
            if exit_status:
                sys.exit(status)
    else:
        sys.modules[name] = importlib.import_module(module or command)
