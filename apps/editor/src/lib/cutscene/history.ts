// The cutscene editor's undo stack: the edits' states themselves (a state
// is small: cutscenes.json's value, the text, the pictures changed, all
// immutable and shared between states). A change records the state after
// it; typing into one field again within a second is the same step. The
// stack also says whether the state is the one last saved.

export class Snapshots<T> {
  private states: T[];
  private labels: string[] = [""];
  private at = 0;
  private saved = 0;
  private lastKey: string | null = null;
  private lastTime = 0;
  private readonly limit: number;
  /** bumped on every change, for a reactive view to follow */
  version = 0;

  constructor(initial: T, limit = 500) {
    this.states = [initial];
    this.limit = limit;
  }

  get current(): T {
    return this.states[this.at]!;
  }

  /**
   * Records `state` as the next step (what `label` did). With a `key`, a
   * change of the same key right after the last one (within `window` ms)
   * replaces that step instead: a word typed is one step, not a step a letter.
   */
  push(label: string, state: T, key: string | null = null, now = Date.now(), window = 1000): void {
    const merge = key !== null && key === this.lastKey && now - this.lastTime < window && this.at === this.states.length - 1 && this.at > 0 && this.saved !== this.at;
    this.lastKey = key;
    this.lastTime = now;
    if (merge) {
      this.states[this.at] = state;
      this.version++;
      return;
    }
    this.states.length = this.at + 1;
    this.labels.length = this.at + 1;
    if (this.saved > this.at) this.saved = -1;
    this.states.push(state);
    this.labels.push(label);
    this.at++;
    if (this.states.length > this.limit + 1) {
      const drop = this.states.length - this.limit - 1;
      this.states.splice(0, drop);
      this.labels.splice(0, drop);
      this.at -= drop;
      this.saved = this.saved >= drop ? this.saved - drop : -1;
    }
    this.version++;
  }

  get canUndo(): boolean {
    return this.at > 0;
  }

  get canRedo(): boolean {
    return this.at < this.states.length - 1;
  }

  get undoLabel(): string | undefined {
    return this.at > 0 ? this.labels[this.at] : undefined;
  }

  get redoLabel(): string | undefined {
    return this.canRedo ? this.labels[this.at + 1] : undefined;
  }

  undo(): T | undefined {
    if (!this.canUndo) return undefined;
    this.at--;
    this.lastKey = null;
    this.version++;
    return this.current;
  }

  redo(): T | undefined {
    if (!this.canRedo) return undefined;
    this.at++;
    this.lastKey = null;
    this.version++;
    return this.current;
  }

  get dirty(): boolean {
    return this.at !== this.saved;
  }

  markSaved(): void {
    this.saved = this.at;
    this.lastKey = null;
    this.version++;
  }

  /** Starts again from `state`, saved. */
  reset(state: T): void {
    this.states = [state];
    this.labels = [""];
    this.at = 0;
    this.saved = 0;
    this.lastKey = null;
    this.version++;
  }
}
