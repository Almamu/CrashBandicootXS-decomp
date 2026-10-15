// The level editor's undo stack: every change to the open room is an Edit
// that knows how to undo and redo itself. The stack also says whether the
// room is as it was last saved (its position is the saved one).

export interface Edit {
  label: string;
  undo(): void;
  redo(): void;
}

export class History {
  private edits: Edit[] = [];
  private at = 0;
  /** the stack's position when the room was saved (-1: a state no longer reachable) */
  private saved = 0;
  private readonly limit: number;
  /** bumped on every change, for a reactive view to follow */
  version = 0;

  constructor(limit = 500) {
    this.limit = limit;
  }

  /** Records an edit already applied. */
  push(e: Edit): void {
    this.edits.length = this.at;
    if (this.saved > this.at) this.saved = -1;
    this.edits.push(e);
    this.at++;
    if (this.edits.length > this.limit) {
      const drop = this.edits.length - this.limit;
      this.edits.splice(0, drop);
      this.at -= drop;
      this.saved = this.saved >= drop ? this.saved - drop : -1;
    }
    this.version++;
  }

  /** Applies an edit and records it. */
  apply(e: Edit): void {
    e.redo();
    this.push(e);
  }

  get canUndo(): boolean {
    return this.at > 0;
  }

  get canRedo(): boolean {
    return this.at < this.edits.length;
  }

  get undoLabel(): string | undefined {
    return this.edits[this.at - 1]?.label;
  }

  get redoLabel(): string | undefined {
    return this.edits[this.at]?.label;
  }

  undo(): boolean {
    const e = this.edits[this.at - 1];
    if (!e) return false;
    e.undo();
    this.at--;
    this.version++;
    return true;
  }

  redo(): boolean {
    const e = this.edits[this.at];
    if (!e) return false;
    e.redo();
    this.at++;
    this.version++;
    return true;
  }

  /** Whether the room differs from what was saved (or opened). */
  get dirty(): boolean {
    return this.at !== this.saved;
  }

  markSaved(): void {
    this.saved = this.at;
    this.version++;
  }

  clear(): void {
    this.edits = [];
    this.at = 0;
    this.saved = 0;
    this.version++;
  }
}
