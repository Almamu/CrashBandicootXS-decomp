#!/usr/bin/env python3
"""tools/apply_mod.py, moved to packages/ (#1167): buildtools.apply_mod,
with crashdata.mods."""
import _packages

_packages.shim(__name__, "buildtools.apply_mod", "crashdata.mods")
