// The web editor (docs/editors.md, "Running the editor"). `pnpm dev` serves it
// with the API proxied to a backend on 127.0.0.1:8765 (`make editor`, or
// CRASH_EDITOR_URL); `pnpm build` writes dist/, which the backend serves and
// the desktop app bundles.
import { svelte } from "@sveltejs/vite-plugin-svelte";
import { defineConfig } from "vitest/config";

export default defineConfig({
  plugins: [svelte()],
  // relative asset paths: the same build works served by the backend and inside the desktop app
  base: "./",
  build: { target: "es2022", sourcemap: true, chunkSizeWarningLimit: 1024 },
  server: {
    host: "127.0.0.1",
    port: 5173,
    strictPort: true,
    proxy: {
      "/api": {
        target: process.env.CRASH_EDITOR_URL ?? "http://127.0.0.1:8765",
        // the backend accepts only its own Host and Origin: the proxy is its page's
        changeOrigin: true,
        configure: (proxy) => proxy.on("proxyReq", (req) => req.removeHeader("origin")),
      },
    },
  },
  test: { environment: "node" },
});
