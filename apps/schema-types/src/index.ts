// @crash/schema-types (#1167, docs/editors.md "The TypeScript workspace"): the
// data files' types, generated from packages/crashdata/schemas/, and the
// backend's typed client.
export type * from "./generated/index.ts";
export { FILE_MATCH, SCHEMA_NAMES } from "./generated/index.ts";
export * from "./client.ts";
export { globToRegExp, schemaFor } from "./glob.ts";
export { parsePointer, formatPointer, getAt } from "./pointer.ts";
