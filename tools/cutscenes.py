#!/usr/bin/env python3
"""tools/cutscenes.py, moved to packages/ (#1167): buildtools.cutscenes,
with crashdata.cutscenes."""
import _packages

_packages.shim(__name__, "buildtools.cutscenes", "crashdata.cutscenes")
