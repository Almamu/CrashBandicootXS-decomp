// The backend this page talks to: its own origin (served by
// tools/editor_server.py, or Vite's dev server's proxy), or, in the desktop
// app, the backend it started (window.__CRASH_EDITOR__, set before the page loads).
import { EditorClient } from "@crash/schema-types/client";

export interface DesktopConfig {
  url: string;
  token: string;
  checkout?: string;
}

declare global {
  interface Window {
    __CRASH_EDITOR__?: DesktopConfig;
  }
}

export const desktop: DesktopConfig | undefined = typeof window !== "undefined" ? window.__CRASH_EDITOR__ : undefined;

export const client = new EditorClient({ baseUrl: desktop?.url, token: desktop?.token });

/** A backend URL an <img> can load (the desktop app's token in the query). */
export function assetUrl(path: string): string {
  const base = desktop?.url?.replace(/\/+$/, "") ?? "";
  if (!desktop?.token) return base + path;
  return `${base}${path}${path.includes("?") ? "&" : "?"}token=${encodeURIComponent(desktop.token)}`;
}
