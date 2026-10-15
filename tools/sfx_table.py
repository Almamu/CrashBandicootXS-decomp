#!/usr/bin/env python3
"""tools/sfx_table.py, moved to packages/ (#1167): buildtools.sfx_table,
with crashdata.audio.sfx_table."""
import _packages

_packages.shim(__name__, "buildtools.sfx_table", "crashdata.audio.sfx_table")
