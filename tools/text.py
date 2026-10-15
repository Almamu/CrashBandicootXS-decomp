#!/usr/bin/env python3
"""tools/text.py, moved to packages/ (#1167): buildtools.text,
with crashdata.text."""
import _packages

_packages.shim(__name__, "buildtools.text", "crashdata.text")
