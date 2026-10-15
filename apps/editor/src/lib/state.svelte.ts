// The editor's state: the project, the files, the open file, the diagnostics.
import { ApiError, type Diagnostic, type FileEntry, type Project, type SchemaIndex, type SchemaName } from "@crash/schema-types";
import { getAt, parsePointer } from "@crash/schema-types";
import { client, hashWith } from "./api.ts";
import type { Json, Schema } from "./schema.ts";

export interface OpenFile {
  path: string;
  schema: SchemaName;
  /** the value as the backend gave it (or as last saved), JSON.stringify'd */
  saved: string;
}

export interface DiagnosticSet {
  source: "data-check" | "save" | "preview";
  title: string;
  ok: boolean;
  diagnostics: Diagnostic[];
  output?: string;
  at: Date;
}

class EditorState {
  projects = $state.raw<Project[]>([]);
  projectId = $state<string>("");
  index = $state.raw<SchemaIndex | null>(null);
  schemas = $state.raw<Partial<Record<SchemaName, Schema>>>({});
  files = $state.raw<FileEntry[]>([]);
  open = $state.raw<OpenFile | null>(null);
  /** the open file's value, edited in place */
  doc = $state<Json>(null);
  /** a JSON pointer in the open file to show (a diagnostic's) */
  focus = $state<string | null>(null);
  focusSeq = $state(0);
  diagnostics = $state.raw<DiagnosticSet | null>(null);
  busy = $state<string | null>(null);
  error = $state<string | null>(null);
  /** what else asks before the project changes (the level editor's unsaved room) */
  projectGuards: (() => boolean)[] = [];

  get project(): Project | undefined {
    return this.projects.find((p) => p.id === this.projectId);
  }

  async init(): Promise<void> {
    await this.run("Connecting", async () => {
      const [projects, index] = await Promise.all([client.projects(), client.schemaIndex()]);
      this.projects = projects;
      this.index = index;
      const names = index.schemas.map((s) => s.schema);
      const loaded = await Promise.all(names.map((n) => client.schema(n)));
      this.schemas = Object.fromEntries(names.map((n, i) => [n, loaded[i] as Schema]));
      const want = new URLSearchParams(location.hash.slice(1)).get("project");
      await this.selectProject(projects.find((p) => p.id === want)?.id ?? projects[0]?.id ?? "");
    });
  }

  async run<T>(what: string, fn: () => Promise<T>): Promise<T | undefined> {
    this.busy = what;
    this.error = null;
    try {
      return await fn();
    } catch (e) {
      this.error = e instanceof Error ? e.message : String(e);
      if (e instanceof ApiError && e.diagnostics.length) {
        this.diagnostics = { source: "save", title: `${what}: ${e.message}`, ok: false, diagnostics: e.diagnostics, at: new Date() };
      }
      return undefined;
    } finally {
      this.busy = null;
    }
  }

  get dirty(): boolean {
    return this.open !== null && JSON.stringify(this.doc) !== this.open.saved;
  }

  confirmDiscard(): boolean {
    return !this.dirty || confirm(`${this.open?.path} has changes that aren't saved. Discard them?`);
  }

  async selectProject(id: string): Promise<void> {
    if (!this.confirmDiscard() || !this.projectGuards.every((g) => g())) return;
    this.projectId = id;
    this.open = null;
    this.doc = null;
    this.files = [];
    this.diagnostics = null;
    history.replaceState(null, "", hashWith("project", id));
    const p = this.project;
    if (!p || !p.ready) return;
    await this.run("Listing the files", async () => {
      this.files = await client.files(id);
    });
  }

  async refreshProjects(): Promise<void> {
    this.projects = await client.projects();
  }

  async addMod(dir: string): Promise<void> {
    await this.run("Adding the mod", async () => {
      const p = await client.addMod(dir);
      await this.refreshProjects();
      await this.selectProject(p.id);
    });
  }

  async applyMod(): Promise<void> {
    const p = this.project;
    if (!p || p.kind !== "mod") return;
    if (p.ready && !confirm(`Apply ${p.mod} again? Its tree (${p.root}) is made again: edits there are lost.`)) return;
    await this.run(`Applying ${p.mod}`, async () => {
      await client.applyMod(p.id);
      await this.refreshProjects();
      this.open = null;
      await this.selectProject(p.id);
    });
  }

  async openFile(path: string, focus: string | null = null): Promise<boolean> {
    if (this.open?.path !== path) {
      if (!this.confirmDiscard()) return false;
      const ok = await this.run(`Opening ${path}`, async () => {
        const f = await client.read(this.projectId, path);
        this.doc = f.data as Json;
        this.open = { path: f.path, schema: f.schema, saved: JSON.stringify(f.data) };
        return true;
      });
      if (!ok) return false;
    }
    this.jump(focus);
    return true;
  }

  /** Shows a pointer's place: its deepest part the value has. */
  jump(pointer: string | null): void {
    if (pointer === null) {
      this.focus = null;
      return;
    }
    let parts = parsePointer(pointer);
    while (parts.length && getAt(this.doc, parts) === undefined) parts = parts.slice(0, -1);
    this.focus = parts.map((p) => "/" + p.replace(/~/g, "~0").replace(/\//g, "~1")).join("");
    this.focusSeq++;
  }

  async save(): Promise<void> {
    const open = this.open;
    if (!open) return;
    const data = $state.snapshot(this.doc as unknown);
    await this.run(`Saving ${open.path}`, async () => {
      const r = await client.save(this.projectId, open.path, data as never);
      this.open = { ...open, saved: JSON.stringify(data) };
      const errors = r.diagnostics.filter((d) => d.severity === "error").length;
      this.diagnostics = {
        source: "save",
        title: r.saved.length
          ? `Saved ${open.path}` + (r.diagnostics.length ? ` (the data's other ${errors} error(s), ${r.diagnostics.length - errors} warning(s) are as they were)` : "")
          : `${open.path}: nothing changed`,
        ok: true,
        diagnostics: r.diagnostics,
        at: new Date(),
      };
    });
  }

  revert(): void {
    if (this.open) this.doc = JSON.parse(this.open.saved) as Json;
  }

  async check(text: boolean): Promise<void> {
    await this.run("Checking the data", async () => {
      const r = await client.check(this.projectId, { text });
      this.diagnostics = {
        source: "data-check",
        title: r.ok ? `data-check: ${this.project?.title} OK` : `data-check: ${r.diagnostics.filter((d) => d.severity === "error").length} error(s)`,
        ok: r.ok,
        diagnostics: r.diagnostics,
        output: r.output,
        at: new Date(),
      };
    });
  }
}

export const editor = new EditorState();
