#!/usr/bin/env python3
"""tools/levels.py, moved to packages/ (#1167): buildtools.levels,
with crashdata.levels."""
import _packages

_packages.shim(__name__, "buildtools.levels", "crashdata.levels")
