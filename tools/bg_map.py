#!/usr/bin/env python3
"""tools/bg_map.py, moved to packages/ (#1167): buildtools.bg_map,
with crashdata.graphics.bg_map."""
import _packages

_packages.shim(__name__, "buildtools.bg_map", "crashdata.graphics.bg_map")
