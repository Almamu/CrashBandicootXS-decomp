#!/usr/bin/env python3
"""tools/dump_entities.py, moved to packages/ (#1167): buildtools.dump_entities,
with crashdata.graphics.dump_entities."""
import _packages

_packages.shim(__name__, "buildtools.dump_entities", "crashdata.graphics.dump_entities")
