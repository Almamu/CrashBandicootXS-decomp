// index.json's globs, matched as crashdata.schemas.schema_for matches them
// (Python's fnmatch: `*` matches any characters, `/` too; `?` one; `[...]` a set).
import { FILE_MATCH, SCHEMA_NAMES, type SchemaName } from "./generated/index.ts";

const cache = new Map<string, RegExp>();

/** fnmatch's pattern as a RegExp. */
export function globToRegExp(glob: string): RegExp {
  let re = cache.get(glob);
  if (re) return re;
  let out = "";
  for (let i = 0; i < glob.length; i++) {
    const c = glob[i]!;
    if (c === "*") out += ".*";
    else if (c === "?") out += ".";
    else if (c === "[") {
      const j = glob.indexOf("]", i + 2);
      if (j < 0) out += "\\[";
      else {
        let set = glob.slice(i + 1, j).replace(/\\/g, "\\\\");
        if (set.startsWith("!")) set = "^" + set.slice(1);
        out += `[${set}]`;
        i = j;
      }
    } else out += c.replace(/[.+^${}()|\\\]]/g, "\\$&");
  }
  re = new RegExp(`^${out}$`, "s");
  cache.set(glob, re);
  return re;
}

/** The schema describing a file (a path from the tree's root, games/<game>/...), or undefined. */
export function schemaFor(path: string, fileMatch: Readonly<Record<string, readonly string[]>> = FILE_MATCH): SchemaName | undefined {
  for (const name of SCHEMA_NAMES) {
    for (const g of fileMatch[name] ?? []) {
      if (globToRegExp(g).test(path) || globToRegExp(g.replace(/\*\*\//g, "")).test(path)) return name;
    }
  }
  return undefined;
}
