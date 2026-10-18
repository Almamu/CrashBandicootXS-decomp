// The cutscene editor's state (docs/editors.md, "The cutscene editor"): the
// project's cutscenes as the backend gave them (crashdata.cutscenes.edit),
// the edit state now (cutscenes.json's value, the text, the pictures
// changed: model.ts), the undo stack of states, what is selected and shown,
// and saving it all back through the backend's cutscene save, which checks
// it like any save (and the text's checks) and keeps it all or none.
import { ApiError, type CutsceneImport, type CutscenesView } from "@crash/schema-types";
import { assetUrl, client, desktop, hashWith } from "../api.ts";
import { editor } from "../state.svelte.ts";
import { Snapshots } from "./history.ts";
import * as M from "./model.ts";
import { checkString, encode, screens, type Glyph, type StringInfo } from "./text.ts";

export interface Play {
  running: boolean;
  ok: boolean;
  text: string;
  images: string[];
}

function emptyState(): M.EditState {
  return { doc: { cutscenes: [], slides: [] }, pages: {}, pictures: {}, newPictures: {} };
}

class CutsceneEditorState {
  view = $state.raw<CutscenesView | null>(null);
  /** the reason there is no view (N-Tranced's cutscenes aren't data) */
  unavailable = $state<string | null>(null);
  state = $state.raw<M.EditState>(emptyState());
  /** the state last saved (or opened) */
  saved = $state.raw<M.EditState>(emptyState());
  /** the manifest's pictures as last saved, by path */
  originals = $state.raw<Record<string, M.PictureEdit>>({});
  history = new Snapshots<M.EditState>(emptyState());
  hv = $state(0);

  cutscene = $state(0);
  slide = $state(0);
  /** the text shown in the slide view: its language, string and screen */
  language = $state<M.Lang>("en");
  string = $state(0);
  screen = $state(0);
  /** the languages the text panel shows side by side */
  shown = $state<Record<M.Lang, boolean>>({ en: true, fr: true, de: true, es: true, it: true, nl: true });
  zoom = $state(2);
  play = $state<Play | null>(null);
  loading = $state(false);

  private projectKey = "";

  get dirty(): boolean {
    void this.hv;
    return this.history.dirty;
  }

  get canUndo(): boolean {
    void this.hv;
    return this.history.canUndo;
  }

  get canRedo(): boolean {
    void this.hv;
    return this.history.canRedo;
  }

  get undoLabel(): string {
    void this.hv;
    return this.history.undoLabel ?? "";
  }

  get redoLabel(): string {
    void this.hv;
    return this.history.redoLabel ?? "";
  }

  get doc(): M.Doc {
    return this.state.doc;
  }

  get current(): M.Cutscene | undefined {
    return this.state.doc.cutscenes[this.cutscene];
  }

  get ref(): M.SlideRef | undefined {
    return this.current?.slides[this.slide];
  }

  get record(): M.SlideRecord | undefined {
    const r = this.ref;
    return r ? M.record(this.state.doc, r.slide) : undefined;
  }

  get page(): M.Str[] | undefined {
    const t = this.ref?.text;
    return t ? this.state.pages[t] : undefined;
  }

  /** Every picture there is: the manifest's and the new ones. */
  get picturePaths(): string[] {
    return [...(this.view?.pictures.map((p) => p.path) ?? []), ...Object.keys(this.state.newPictures).filter((p) => !this.view?.pictures.some((x) => x.path === p))];
  }

  picture(path: string | undefined): M.PictureEdit | null {
    if (!path) return null;
    return this.state.pictures[path] ?? this.state.newPictures[path] ?? this.originals[path] ?? null;
  }

  confirmDiscard(): boolean {
    return !this.dirty || confirm("The cutscenes have changes that aren't saved. Discard them?");
  }

  // -- opening ----------------------------------------------------------------------

  /** Follows the editor's project: a new one reads its cutscenes. */
  async syncProject(id: string, ready: boolean): Promise<void> {
    const key = `${id}:${ready}`;
    if (key === this.projectKey) return;
    this.projectKey = key;
    this.view = null;
    this.unavailable = null;
    this.reset(emptyState(), {});
    if (ready) await this.load();
  }

  private reset(state: M.EditState, originals: Record<string, M.PictureEdit>): void {
    this.state = state;
    this.saved = state;
    this.originals = originals;
    this.history.reset(state);
    this.hv = this.history.version;
  }

  async load(): Promise<void> {
    const id = editor.projectId;
    this.loading = true;
    try {
      await editor.run("Reading the cutscenes", async () => {
        let view: CutscenesView;
        try {
          view = await client.cutscenes(id);
        } catch (e) {
          if (e instanceof ApiError && e.status === 404) {
            this.unavailable = e.message;
            return;
          }
          throw e;
        }
        const pics = await Promise.all(view.pictures.map((p) => client.cutscenePicture(id, p.path)));
        const originals: Record<string, M.PictureEdit> = {};
        view.pictures.forEach((p, i) => (originals[p.path] = { palette: pics[i]!.palette, pixels: pics[i]!.pixels }));
        const pages: Record<string, M.Str[]> = {};
        for (const [k, ss] of Object.entries(view.pages)) pages[k] = ss.map((s) => Object.fromEntries(M.LANGS.map((l) => [l, s[l]])) as M.Str);
        this.view = view;
        this.reset({ doc: structuredClone(view.cutscenes) as unknown as M.Doc, pages, pictures: {}, newPictures: {} }, originals);
        // the page's hash may name a cutscene, a slide and a language (#cutscene=1&slide=2&lang=fr)
        const q = new URLSearchParams(location.hash.slice(1));
        const n = view.cutscenes.cutscenes.length;
        this.cutscene = Math.max(0, Math.min(Number(q.get("cutscene") ?? this.cutscene) || 0, n - 1));
        const m = view.cutscenes.cutscenes[this.cutscene]!.slides.length;
        this.slide = Math.max(0, Math.min(Number(q.get("slide") ?? 0) || 0, m - 1));
        const lang = q.get("lang");
        if (lang && (M.LANGS as readonly string[]).includes(lang)) this.language = lang as M.Lang;
        this.string = 0;
        this.screen = 0;
      });
    } finally {
      this.loading = false;
    }
  }

  // -- edits ----------------------------------------------------------------------------

  /** Applies an edit (a function from the state to the next): an undo step, unless nothing changes. */
  apply(label: string, f: (s: M.EditState) => M.EditState, key: string | null = null): boolean {
    let next: M.EditState;
    try {
      next = f(this.state);
    } catch (e) {
      editor.error = e instanceof Error ? e.message : String(e);
      return false;
    }
    if (next === this.state) return false;
    this.state = next;
    this.history.push(label, next, key);
    this.hv = this.history.version;
    this.clampSelection();
    return true;
  }

  undo(): void {
    const s = this.history.undo();
    if (s) this.state = s;
    this.hv = this.history.version;
    this.clampSelection();
  }

  redo(): void {
    const s = this.history.redo();
    if (s) this.state = s;
    this.hv = this.history.version;
    this.clampSelection();
  }

  private clampSelection(): void {
    const n = this.state.doc.cutscenes.length;
    if (this.cutscene >= n) this.cutscene = Math.max(0, n - 1);
    const m = this.current?.slides.length ?? 0;
    if (this.slide >= m) this.slide = Math.max(0, m - 1);
    const k = this.page?.length ?? 0;
    if (this.string >= k) this.string = Math.max(0, k - 1);
  }

  select(ci: number, si = 0): void {
    this.cutscene = ci;
    this.selectSlide(si);
  }

  selectSlide(si: number): void {
    this.slide = si;
    this.string = 0;
    this.screen = 0;
    window.history.replaceState(null, "", hashWith("cutscene", String(this.cutscene)));
    window.history.replaceState(null, "", hashWith("slide", String(si)));
  }

  // -- the text as the game shows it ---------------------------------------------------------

  info(text: string): StringInfo | null {
    const v = this.view;
    return v ? checkString(text, v.font, v.box) : null;
  }

  /** The screens of the slide's text in the language shown: [string, screen, glyphs] for each key press. */
  get steps(): { string: number; screen: number; glyphs: Glyph[] }[] {
    const v = this.view;
    const page = this.page;
    if (!v || !page) return [{ string: 0, screen: 0, glyphs: [] }];
    const out: { string: number; screen: number; glyphs: Glyph[] }[] = [];
    page.forEach((s, j) => {
      screens(encode(s[this.language]).bytes, v.font, v.box).forEach((glyphs, k) => out.push({ string: j, screen: k, glyphs }));
    });
    return out.length ? out : [{ string: 0, screen: 0, glyphs: [] }];
  }

  /** The key presses of the whole cutscene in the language shown (the PC port's headless play has a picture for each). */
  get cutsceneSteps(): number {
    const v = this.view;
    const c = this.current;
    if (!v || !c) return 0;
    let n = 0;
    for (const s of c.slides) {
      const page = s.text ? this.state.pages[s.text] : undefined;
      if (!page) n++;
      else for (const st of page) n += screens(encode(st[this.language]).bytes, v.font, v.box).length;
    }
    return n;
  }

  // -- pictures ----------------------------------------------------------------------------------

  /** A PNG file made into a picture by the backend: on its own colours, or mapped to `palette`. */
  async importFile(file: File, palette?: number[]): Promise<CutsceneImport | undefined> {
    const buf = new Uint8Array(await file.arrayBuffer());
    return editor.run(`Importing ${file.name}`, () => client.importCutscenePicture(editor.projectId, M.base64(buf), palette));
  }

  /** Replaces a picture's pixels and colours (every slide that shows it). */
  replacePicture(path: string, pic: M.PictureEdit): void {
    this.apply(`Replace ${path}`, (s) => {
      if (s.newPictures[path]) return { ...s, newPictures: { ...s.newPictures, [path]: { ...s.newPictures[path]!, ...pic } } };
      return { ...s, pictures: { ...s.pictures, [path]: pic } };
    });
  }

  /** A new picture, shown by the selected slide (its own record if others show its record). */
  addPicture(name: string, about: string, pic: M.PictureEdit): void {
    const path = M.newPicturePath(this.picturePaths, name);
    const { cutscene: ci, slide: si } = this;
    this.apply(`New picture ${path}`, (s) => {
      let next: M.EditState = { ...s, newPictures: { ...s.newPictures, [path]: { ...pic, about } } };
      const ref = next.doc.cutscenes[ci]?.slides[si];
      if (!ref) return next;
      if (M.recordUsers(next.doc, ref.slide).length > 1) next = M.ownRecord(next, ci, si);
      const name2 = next.doc.cutscenes[ci]!.slides[si]!.slide;
      return M.setRecord(next, name2, { picture: path });
    });
  }

  // -- saving ------------------------------------------------------------------------------------------

  saveBody(): M.SaveBody {
    return M.saveBody(this.saved, this.state, this.originals, new Set(this.view?.pictures.map((p) => p.path) ?? []));
  }

  async save(): Promise<void> {
    const body = this.saveBody();
    const now = this.state;
    await editor.run("Saving the cutscenes", async () => {
      const r = await client.saveCutscenes(editor.projectId, body as never);
      // what is saved is now the state as opened: the pictures too
      const originals = { ...this.originals };
      for (const [p, pic] of Object.entries(now.pictures)) originals[p] = pic;
      for (const [p, pic] of Object.entries(now.newPictures)) originals[p] = { palette: pic.palette, pixels: pic.pixels };
      this.originals = originals;
      this.saved = now;
      this.history.markSaved();
      this.hv = this.history.version;
      if (body.newPictures?.length && this.view) {
        // the manifest has them now
        const view = await client.cutscenes(editor.projectId);
        this.view = view;
      }
      const errors = r.diagnostics.filter((d) => d.severity === "error").length;
      editor.diagnostics = {
        source: "save",
        title: r.saved.length ? `Saved ${r.saved.join(", ")}` + (errors ? ` (the data's other ${errors} error(s) are as they were)` : "") : "The cutscenes: nothing changed",
        ok: true,
        diagnostics: r.diagnostics,
        at: new Date(),
      };
    });
  }

  async revert(): Promise<void> {
    if (!this.confirmDiscard()) return;
    this.history.reset(this.saved);
    await this.load();
  }

  // -- playing it ------------------------------------------------------------------------------------------

  async runPreview(): Promise<void> {
    const c = this.current;
    if (!c) return;
    if (this.dirty && !confirm("The preview builds the saved data: the changes aren't saved. Play anyway?")) return;
    const headless = !desktop;
    this.play = { running: true, ok: true, images: [], text: headless ? "Building the PC port and playing the cutscene headless (A every 60 frames)…" : "Building the PC port and starting it…" };
    try {
      const r = await client.preview(editor.projectId, { cutscene: this.cutscene, language: this.language, headless });
      if ("started" in r) this.play = { running: false, ok: true, images: [], text: `Started in its window (${r.command.join(" ")}); its output: ${r.log}. The game plays ${c.constant} at boot, then boots on.` };
      else {
        const errs = r.diagnostics.filter((d) => d.severity === "error");
        const steps = Number((r.preview as { steps?: number } | undefined)?.steps ?? 0);
        this.play = {
          running: false,
          ok: r.ok,
          text: r.ok ? `${c.constant} in the PC port: a picture before each of the ${steps} key presses, then the boot going on.` : errs.map((d) => d.message).join("\n") || r.output.slice(-2000),
          images: (r.images ?? []).map((u) => assetUrl(u)),
        };
      }
    } catch (e) {
      this.play = { running: false, ok: false, images: [], text: e instanceof Error ? e.message : String(e) };
    }
  }
}

export const cutscenes = new CutsceneEditorState();

// switching projects asks about the cutscenes' changes too
editor.projectGuards.push(() => cutscenes.confirmDiscard());
