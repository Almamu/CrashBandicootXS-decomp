// A typed client of the editors' backend, tools/editor_server.py (#1167,
// docs/editors.md "The editor's backend"). The browser editor calls it on
// its own origin; the desktop app gives it the backend's URL and token.
import type { SchemaFiles, SchemaName } from "./generated/index.ts";

/** One problem with the data (crashdata.common.diagnostics.Diagnostic). */
export interface Diagnostic {
  file: string | null;
  /** a JSON pointer (RFC 6901) into the file, or null */
  pointer: string | null;
  line: number | null;
  col: number | null;
  severity: "error" | "warning";
  code: string;
  message: string;
}

/** A data tool's `--json` report (docs/editors.md, "Errors"). */
export interface Report {
  version: number;
  tool: string;
  ok: boolean;
  diagnostics: Diagnostic[];
  output: string;
}

export interface PreviewReport extends Report {
  preview?: Record<string, unknown>;
  /** a headless preview's picture, on this server */
  image?: string | null;
}

export interface PreviewStarted {
  ok: true;
  started: true;
  pid: number;
  command: string[];
  log: string;
}

export interface PreviewOptions {
  /** gLevelTable's index, a constant, a name or a room folder */
  level: string | number;
  /** an index in the level's "rooms" or a folder */
  room?: string | number;
  /** run headless and give back a picture, instead of opening the game's window */
  headless?: boolean;
  frames?: number;
  noBuild?: boolean;
}

export interface Health {
  ok: boolean;
  version: number;
  tree: string;
  python: string;
  /** whether the server has the built web editor to serve */
  editor: boolean;
}

export interface Project {
  /** "xs", "n2", or "mod:<name>" */
  id: string;
  kind: "game" | "mod";
  game: "xs" | "n2";
  gameTitle: string;
  title: string;
  /** the project's tree, from the server's tree's root */
  root: string;
  /** a mod's tree exists (it has been applied) */
  ready: boolean;
  mod?: string;
}

export interface SchemaIndex {
  $comment?: string;
  schemas: { schema: SchemaName; fileMatch: string[] }[];
}

export interface FileEntry {
  path: string;
  schema: SchemaName;
}

export interface DataFile<N extends SchemaName = SchemaName> {
  path: string;
  schema: N;
  data: SchemaFiles[N];
}

export interface SaveResult {
  ok: true;
  /** the files that changed (a file saved as it was isn't written) */
  saved: string[];
  /** the model's checks after the save (warnings, and errors that were there before) */
  diagnostics: Diagnostic[];
}

export interface ApplyResult {
  ok: true;
  project: Project;
  report: Report;
}

/** A JSON Schema, as the backend serves it. */
export type JsonSchema = Record<string, unknown>;

/** The backend's answer to a request it refused. */
export class ApiError extends Error {
  readonly status: number;
  readonly diagnostics: Diagnostic[];

  constructor(status: number, message: string, diagnostics: Diagnostic[] = []) {
    super(message);
    this.name = "ApiError";
    this.status = status;
    this.diagnostics = diagnostics;
  }
}

export interface ClientOptions {
  /** the backend's URL ("" or omitted: the page's own origin) */
  baseUrl?: string;
  /** the desktop app's token (tools/editor_server.py --token) */
  token?: string;
  fetch?: typeof fetch;
}

export class EditorClient {
  readonly baseUrl: string;
  readonly token: string | undefined;
  private readonly fetchImpl: typeof fetch;

  constructor(options: ClientOptions = {}) {
    this.baseUrl = (options.baseUrl ?? "").replace(/\/+$/, "");
    this.token = options.token;
    this.fetchImpl = options.fetch ?? ((input, init) => globalThis.fetch(input, init));
  }

  /** A URL of the backend, with its query. */
  url(path: string, query: Record<string, string | undefined> = {}): string {
    const q = Object.entries(query)
      .filter((kv): kv is [string, string] => kv[1] !== undefined)
      .map(([k, v]) => `${encodeURIComponent(k)}=${encodeURIComponent(v)}`)
      .join("&");
    return `${this.baseUrl}${path}${q ? `?${q}` : ""}`;
  }

  private headers(write: boolean): Record<string, string> {
    const h: Record<string, string> = { Accept: "application/json" };
    if (write) {
      h["Content-Type"] = "application/json";
      h["X-Crash-Editor"] = "1";
    }
    if (this.token) h["Authorization"] = `Bearer ${this.token}`;
    return h;
  }

  async request<T>(method: "GET" | "PUT" | "POST", path: string, body?: unknown, query?: Record<string, string | undefined>): Promise<T> {
    const write = method !== "GET";
    const res = await this.fetchImpl(this.url(path, query), {
      method,
      headers: this.headers(write),
      body: body === undefined ? undefined : JSON.stringify(body),
    });
    const text = await res.text();
    let json: unknown = undefined;
    try {
      json = text ? JSON.parse(text) : undefined;
    } catch {
      throw new ApiError(res.status, `${method} ${path}: not JSON (${res.status})`);
    }
    if (!res.ok) {
      const e = (json ?? {}) as { error?: string; diagnostics?: Diagnostic[] };
      throw new ApiError(res.status, e.error ?? `${method} ${path}: ${res.status}`, e.diagnostics ?? []);
    }
    return json as T;
  }

  private project(id: string, rest: string): string {
    return `/api/projects/${encodeURIComponent(id)}/${rest}`;
  }

  health(): Promise<Health> {
    return this.request("GET", "/api/health");
  }

  async projects(): Promise<Project[]> {
    return (await this.request<{ projects: Project[] }>("GET", "/api/projects")).projects;
  }

  /** Adds a mod (a folder with a mod.py) to the projects. */
  addMod(dir: string): Promise<Project> {
    return this.request("POST", "/api/projects", { mod: dir });
  }

  /** Applies a mod to its tree (build/mods/<name>), as `make mod`: the tree's edits are lost. */
  applyMod(id: string): Promise<ApplyResult> {
    return this.request("POST", this.project(id, "apply"), {});
  }

  schemaIndex(): Promise<SchemaIndex> {
    return this.request("GET", "/api/schemas");
  }

  schema(name: SchemaName): Promise<JsonSchema> {
    return this.request("GET", `/api/schemas/${encodeURIComponent(name)}`);
  }

  async files(id: string): Promise<FileEntry[]> {
    return (await this.request<{ files: FileEntry[] }>("GET", this.project(id, "files"))).files;
  }

  read<N extends SchemaName = SchemaName>(id: string, path: string): Promise<DataFile<N>> {
    return this.request("GET", this.project(id, "file"), undefined, { path });
  }

  /** Saves one file: rejected (an ApiError with diagnostics) if its schema or the model's checks refuse it. */
  save<N extends SchemaName>(id: string, path: string, data: SchemaFiles[N]): Promise<SaveResult> {
    return this.request("PUT", this.project(id, "file"), { data }, { path });
  }

  /** Saves several files together: all, or none. */
  saveMany(id: string, files: { path: string; data: unknown }[]): Promise<SaveResult> {
    return this.request("POST", this.project(id, "save"), { files });
  }

  /** `make data-check`'s report for the project (text: the translations too, which needs gettext). */
  check(id: string, options: { text?: boolean } = {}): Promise<Report> {
    return this.request("GET", this.project(id, "check"), undefined, { text: options.text === false ? "0" : "1" });
  }

  preview(id: string, options: PreviewOptions & { headless: true }): Promise<PreviewReport>;
  preview(id: string, options: PreviewOptions): Promise<PreviewReport | PreviewStarted>;
  preview(id: string, options: PreviewOptions): Promise<PreviewReport | PreviewStarted> {
    return this.request("POST", this.project(id, "preview"), options);
  }

  /** The URL of any file of the project's game directory (a picture, a palette), as it is. */
  rawUrl(id: string, path: string): string {
    return this.url(this.project(id, "raw"), { path });
  }
}
