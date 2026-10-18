// The cutscene editor's model (docs/editors.md, "The cutscene editor"):
// cutscenes.json's value, the pages' text and the pictures changed, and the
// edits on them, each a pure function from a state to the next (the undo
// stack keeps the states). The rules are the data's (docs/data.md,
// "Cutscenes"): a cutscene's slide names a slide record and maybe a page of
// text; a record or a page several slides show is shared; every record and
// every page is shown by a slide (tools/cutscenes.py check), so what no
// slide shows any more goes; a cutscene has one slide or more, and its id is
// its place (levels.json's "on_clear" and the code name it), so only a new
// last one can go.

export const LANGS = ["en", "fr", "de", "es", "it", "nl"] as const;
export type Lang = (typeof LANGS)[number];
export type Str = Record<Lang, string>;

/** struct cutscene_slide's fields after the picture (crashdata.cutscenes.SLIDE_FIELDS). */
export interface SlideRecord {
  name: string;
  picture: string;
  wait: number;
  fade: number;
  fade_after: number;
  buttons: number;
  duck_music: number;
  rearm_sfx: number;
  song: string;
  sfx: string;
}

export interface SlideRef {
  slide: string;
  text?: string;
}

export interface Cutscene {
  constant: string;
  about?: string;
  slides: SlideRef[];
}

export interface Doc {
  cutscenes: Cutscene[];
  slides: SlideRecord[];
  rom_order?: Record<string, string[]>;
}

export interface PictureEdit {
  palette: number[];
  pixels: Uint8Array;
}

export interface NewPicture extends PictureEdit {
  about: string;
}

export interface EditState {
  doc: Doc;
  pages: Record<string, Str[]>;
  /** pictures of the manifest whose pixels or colours change */
  pictures: Record<string, PictureEdit>;
  /** pictures to add to the manifest, by path */
  newPictures: Record<string, NewPicture>;
}

export interface Where {
  ci: number;
  si: number;
}

export const PLACEHOLDER = "...";

function clone<T>(v: T): T {
  return structuredClone(v);
}

export function same(a: unknown, b: unknown): boolean {
  return JSON.stringify(a) === JSON.stringify(b);
}

export function record(doc: Doc, name: string): SlideRecord | undefined {
  return doc.slides.find((s) => s.name === name);
}

/** The slides that show a record. */
export function recordUsers(doc: Doc, name: string): Where[] {
  const out: Where[] = [];
  doc.cutscenes.forEach((c, ci) => c.slides.forEach((s, si) => s.slide === name && out.push({ ci, si })));
  return out;
}

/** The slides that show a page. */
export function pageUsers(doc: Doc, key: string): Where[] {
  const out: Where[] = [];
  doc.cutscenes.forEach((c, ci) => c.slides.forEach((s, si) => s.text === key && out.push({ ci, si })));
  return out;
}

export function uniqueName(base: string, taken: Iterable<string>): string {
  const set = new Set(taken);
  if (!set.has(base)) return base;
  const stem = base.replace(/_\d+$/, "");
  for (let n = 2; ; n++) if (!set.has(`${stem}_${n}`)) return `${stem}_${n}`;
}

/** A page key: cutsceneNN.pageP. */
export function pageKey(ci: number, p: number): string {
  return `cutscene${String(ci).padStart(2, "0")}.page${p}`;
}

/** A new page key for cutscene ci: the first page number it has no page of. */
export function newPageKey(state: EditState, ci: number): string {
  const used = new Set([...Object.keys(state.pages), ...state.doc.cutscenes.flatMap((c) => c.slides.map((s) => s.text ?? ""))]);
  for (let p = 0; ; p++) if (!used.has(pageKey(ci, p))) return pageKey(ci, p);
}

export function placeholder(text = PLACEHOLDER): Str {
  return Object.fromEntries(LANGS.map((l) => [l, text])) as Str;
}

/** Drops the records and pages no slide shows any more. */
export function prune(state: EditState): EditState {
  const records = new Set(state.doc.cutscenes.flatMap((c) => c.slides.map((s) => s.slide)));
  const pages = new Set(state.doc.cutscenes.flatMap((c) => c.slides.flatMap((s) => (s.text ? [s.text] : []))));
  const slides = state.doc.slides.filter((s) => records.has(s.name));
  const keep = Object.fromEntries(Object.entries(state.pages).filter(([k]) => pages.has(k)));
  if (slides.length === state.doc.slides.length && Object.keys(keep).length === Object.keys(state.pages).length) return state;
  return { ...state, doc: { ...state.doc, slides }, pages: keep };
}

function withCutscene(state: EditState, ci: number, f: (c: Cutscene) => Cutscene): EditState {
  const cutscenes = state.doc.cutscenes.map((c, i) => (i === ci ? f(c) : c));
  return { ...state, doc: { ...state.doc, cutscenes } };
}

/** Moves a cutscene's slide from one place to another. */
export function moveSlide(state: EditState, ci: number, from: number, to: number): EditState {
  const c = state.doc.cutscenes[ci];
  if (!c || from === to || from < 0 || to < 0 || from >= c.slides.length || to >= c.slides.length) return state;
  return withCutscene(state, ci, (c) => {
    const slides = [...c.slides];
    const [s] = slides.splice(from, 1);
    slides.splice(to, 0, s!);
    return { ...c, slides };
  });
}

/** Deletes a slide; the record and the page go too if no other slide shows them. */
export function deleteSlide(state: EditState, ci: number, si: number): EditState {
  const c = state.doc.cutscenes[ci];
  if (!c || !c.slides[si]) return state;
  if (c.slides.length === 1) throw new Error("a cutscene has one slide or more: add another first");
  return prune(withCutscene(state, ci, (c) => ({ ...c, slides: c.slides.filter((_, i) => i !== si) })));
}

/** Duplicates a slide after it: the same record (shared), and a copy of its page (its own). */
export function duplicateSlide(state: EditState, ci: number, si: number): EditState {
  const c = state.doc.cutscenes[ci];
  const s = c?.slides[si];
  if (!c || !s) return state;
  const copy: SlideRef = { slide: s.slide };
  let pages = state.pages;
  if (s.text) {
    copy.text = newPageKey(state, ci);
    pages = { ...pages, [copy.text]: clone(state.pages[s.text] ?? [placeholder()]) };
  }
  const next = withCutscene({ ...state, pages }, ci, (c) => {
    const slides = [...c.slides];
    slides.splice(si + 1, 0, copy);
    return { ...c, slides };
  });
  return next;
}

/** Adds a slide at `at`: showing an existing record, or a new copy of `like` (a record) with `picture`; with a new page or none. */
export function addSlide(state: EditState, ci: number, at: number, opts: { record?: string; like?: SlideRecord; picture?: string; text?: boolean }): EditState {
  let doc = state.doc;
  let name = opts.record;
  if (!name || !record(doc, name)) {
    const like = opts.like ?? doc.slides[0];
    if (!like) throw new Error("no slide record to start from");
    name = uniqueName(like.name, doc.slides.map((s) => s.name));
    doc = { ...doc, slides: [...doc.slides, { ...like, name, picture: opts.picture ?? like.picture }] };
  }
  const ref: SlideRef = { slide: name };
  let pages = state.pages;
  let next: EditState = { ...state, doc };
  if (opts.text) {
    ref.text = newPageKey(next, ci);
    pages = { ...pages, [ref.text]: [placeholder()] };
  }
  next = { ...next, pages };
  return withCutscene(next, ci, (c) => {
    const slides = [...c.slides];
    slides.splice(Math.max(0, Math.min(at, slides.length)), 0, ref);
    return { ...c, slides };
  });
}

/** Gives a slide a page of text (one string, "..." in every language), or takes its page away. */
export function setHasText(state: EditState, ci: number, si: number, on: boolean): EditState {
  const s = state.doc.cutscenes[ci]?.slides[si];
  if (!s || on === !!s.text) return state;
  if (on) {
    const key = newPageKey(state, ci);
    const next = { ...state, pages: { ...state.pages, [key]: [placeholder()] } };
    return withCutscene(next, ci, (c) => ({ ...c, slides: c.slides.map((x, i) => (i === si ? { ...x, text: key } : x)) }));
  }
  return prune(withCutscene(state, ci, (c) => ({ ...c, slides: c.slides.map((x, i) => (i === si ? { slide: x.slide } : x)) })));
}

/** Gives a slide its own copy of its record (a record other slides show too). */
export function ownRecord(state: EditState, ci: number, si: number): EditState {
  const s = state.doc.cutscenes[ci]?.slides[si];
  const r = s && record(state.doc, s.slide);
  if (!s || !r) return state;
  const name = uniqueName(r.name, state.doc.slides.map((x) => x.name));
  const doc = { ...state.doc, slides: [...state.doc.slides, { ...r, name }] };
  return withCutscene({ ...state, doc }, ci, (c) => ({ ...c, slides: c.slides.map((x, i) => (i === si ? { ...x, slide: name } : x)) }));
}

/** Gives a slide its own copy of its page (a page other slides show too). */
export function ownPage(state: EditState, ci: number, si: number): EditState {
  const s = state.doc.cutscenes[ci]?.slides[si];
  if (!s?.text) return state;
  const key = newPageKey(state, ci);
  const next = { ...state, pages: { ...state.pages, [key]: clone(state.pages[s.text] ?? [placeholder()]) } };
  return withCutscene(next, ci, (c) => ({ ...c, slides: c.slides.map((x, i) => (i === si ? { ...x, text: key } : x)) }));
}

/** Changes a record's fields (every slide that shows it). A new name is followed by the slides. */
export function setRecord(state: EditState, name: string, patch: Partial<SlideRecord>): EditState {
  const r = record(state.doc, name);
  if (!r) return state;
  if (patch.name !== undefined && patch.name !== name) {
    if (!patch.name || record(state.doc, patch.name)) throw new Error(`a slide record named ${patch.name || "(nothing)"} is there already, or the name is empty`);
  }
  const next = { ...r, ...patch };
  const slides = state.doc.slides.map((s) => (s.name === name ? next : s));
  const cutscenes = next.name === name ? state.doc.cutscenes : state.doc.cutscenes.map((c) => ({ ...c, slides: c.slides.map((s) => (s.slide === name ? { ...s, slide: next.name } : s)) }));
  return { ...state, doc: { ...state.doc, slides, cutscenes } };
}

/** A slide shows another record. */
export function setSlideRecord(state: EditState, ci: number, si: number, name: string): EditState {
  if (!record(state.doc, name)) return state;
  return prune(withCutscene(state, ci, (c) => ({ ...c, slides: c.slides.map((x, i) => (i === si ? { ...x, slide: name } : x)) })));
}

export function setString(state: EditState, page: string, i: number, lang: Lang, text: string): EditState {
  const strings = state.pages[page];
  if (!strings?.[i] || strings[i][lang] === text) return state;
  const next = strings.map((s, k) => (k === i ? { ...s, [lang]: text } : s));
  return { ...state, pages: { ...state.pages, [page]: next } };
}

/** Adds a string to a page at `at` ("..." in every language, or `like`'s). */
export function addString(state: EditState, page: string, at: number, like?: Str): EditState {
  const strings = state.pages[page];
  if (!strings) return state;
  const next = [...strings];
  next.splice(Math.max(0, Math.min(at, next.length)), 0, like ? { ...like } : placeholder());
  return { ...state, pages: { ...state.pages, [page]: next } };
}

export function deleteString(state: EditState, page: string, i: number): EditState {
  const strings = state.pages[page];
  if (!strings?.[i]) return state;
  if (strings.length === 1) throw new Error("a page has one string or more: take the slide's text away instead");
  return { ...state, pages: { ...state.pages, [page]: strings.filter((_, k) => k !== i) } };
}

export function moveString(state: EditState, page: string, from: number, to: number): EditState {
  const strings = state.pages[page];
  if (!strings || from === to || !strings[from] || to < 0 || to >= strings.length) return state;
  const next = [...strings];
  const [s] = next.splice(from, 1);
  next.splice(to, 0, s!);
  return { ...state, pages: { ...state.pages, [page]: next } };
}

export const CONSTANT = /^CUTSCENE_[A-Z0-9_]+$/;

/** A new cutscene at the end: its first slide shows `record`, without text. */
export function addCutscene(state: EditState, constant: string, recordName: string, about?: string): EditState {
  if (!CONSTANT.test(constant) || constant === "CUTSCENE_COUNT") throw new Error(`${constant}: a cutscene's constant is CUTSCENE_<NAME> (capitals, digits, _)`);
  if (state.doc.cutscenes.some((c) => c.constant === constant)) throw new Error(`${constant} is there already`);
  if (state.doc.cutscenes.length >= 100) throw new Error("the text's keys have room for 100 cutscenes (cutsceneNN)");
  if (!record(state.doc, recordName)) throw new Error(`no slide record ${recordName}`);
  const c: Cutscene = { constant, ...(about ? { about } : {}), slides: [{ slide: recordName }] };
  return { ...state, doc: { ...state.doc, cutscenes: [...state.doc.cutscenes, c] } };
}

/** Why a cutscene can't be deleted, or null: only the last one, and only when nothing plays it. */
export function cannotDelete(state: EditState, ci: number, references: string[]): string | null {
  if (ci !== state.doc.cutscenes.length - 1) return "only the last cutscene can go: a cutscene's id is its place, and levels.json and the code name the ids";
  if (references.length) return `it is played by ${references.join(", ")}`;
  if (state.doc.cutscenes.length === 1) return "the game has one cutscene or more";
  return null;
}

export function deleteCutscene(state: EditState, ci: number, references: string[]): EditState {
  const why = cannotDelete(state, ci, references);
  if (why) throw new Error(why);
  const constant = state.doc.cutscenes[ci]!.constant;
  const doc = { ...state.doc, cutscenes: state.doc.cutscenes.filter((_, i) => i !== ci) };
  if (doc.rom_order) doc.rom_order = Object.fromEntries(Object.entries(doc.rom_order).map(([k, v]) => [k, v.filter((n) => n !== constant)]));
  return prune({ ...state, doc });
}

/** A new picture's path: cutscenes/NN_<name>.png, NN after the pictures there are. */
export function newPicturePath(paths: Iterable<string>, name: string): string {
  let max = -1;
  for (const p of paths) {
    const m = /^cutscenes\/(\d+)_/.exec(p);
    if (m) max = Math.max(max, Number(m[1]));
  }
  const slug = name.toLowerCase().replace(/[^a-z0-9]+/g, "_").replace(/^_+|_+$/g, "") || "picture";
  return `cutscenes/${String(max + 1).padStart(2, "0")}_${slug}.png`;
}

function samePicture(a: PictureEdit | undefined, b: PictureEdit | undefined): boolean {
  if (!a || !b) return a === b;
  if (a.pixels.length !== b.pixels.length || !same(a.palette, b.palette)) return false;
  for (let i = 0; i < a.pixels.length; i++) if (a.pixels[i] !== b.pixels[i]) return false;
  return true;
}

export function base64(bytes: Uint8Array): string {
  let s = "";
  for (let i = 0; i < bytes.length; i += 0x8000) s += String.fromCharCode(...bytes.subarray(i, i + 0x8000));
  return btoa(s);
}

export function fromBase64(s: string): Uint8Array {
  const bin = atob(s);
  const out = new Uint8Array(bin.length);
  for (let i = 0; i < bin.length; i++) out[i] = bin.charCodeAt(i);
  return out;
}

export interface SaveBody {
  cutscenes?: Doc;
  pages?: Record<string, Str[] | null>;
  pictures?: Record<string, { palette: number[]; pixels: string }>;
  newPictures?: { path: string; about: string; palette: number[]; pixels: string }[];
}

/**
 * What a save sends: what differs from `saved` (the state last saved), the
 * pictures that differ from `originals` (the manifest's, as last saved), and
 * the new pictures not in `existing` yet.
 */
export function saveBody(saved: EditState, now: EditState, originals: Record<string, PictureEdit>, existing: Set<string>): SaveBody {
  const body: SaveBody = {};
  if (!same(saved.doc, now.doc)) body.cutscenes = now.doc;
  const pages: Record<string, Str[] | null> = {};
  for (const [k, v] of Object.entries(now.pages)) if (!same(saved.pages[k], v)) pages[k] = v;
  for (const k of Object.keys(saved.pages)) if (!(k in now.pages)) pages[k] = null;
  if (Object.keys(pages).length) body.pages = pages;
  const pictures: NonNullable<SaveBody["pictures"]> = {};
  const all: [string, PictureEdit][] = [...Object.entries(now.pictures), ...Object.entries(now.newPictures).filter(([p]) => existing.has(p))];
  for (const [p, pic] of all) if (!samePicture(pic, originals[p])) pictures[p] = { palette: pic.palette, pixels: base64(pic.pixels) };
  if (Object.keys(pictures).length) body.pictures = pictures;
  const shown = new Set(now.doc.slides.map((s) => s.picture));
  const fresh = Object.entries(now.newPictures).filter(([p]) => !existing.has(p) && shown.has(p));
  if (fresh.length) body.newPictures = fresh.map(([path, n]) => ({ path, about: n.about, palette: n.palette, pixels: base64(n.pixels) }));
  return body;
}

/** Whether a save would send nothing. */
export function isEmptyBody(b: SaveBody): boolean {
  return !b.cutscenes && !b.pages && !b.pictures && !b.newPictures;
}
