#!/usr/bin/env python3
"""tools/data_check.py (#1167): buildtools.data_check, with crashdata.datacheck:
`make data-check`, every check of the data without building."""
import _packages

_packages.shim(__name__, "buildtools.data_check", "crashdata.datacheck")
