// The editors' workspace's lint (apps/*; docs/editors.md): `pnpm lint`.
import js from "@eslint/js";
import svelte from "eslint-plugin-svelte";
import globals from "globals";
import ts from "typescript-eslint";

export default ts.config(
  {
    ignores: [
      "**/node_modules/",
      "**/dist/",
      "**/src/generated/",
      "apps/desktop/src-tauri/",
      // the rest of the tree isn't the workspace's
      "asm/",
      "build/",
      "engine/",
      "games/",
      "lib/",
      "mods/",
      "packages/",
      "platform/",
      "tools/",
    ],
  },
  js.configs.recommended,
  ...ts.configs.recommended,
  ...svelte.configs["flat/recommended"],
  {
    languageOptions: { globals: { ...globals.browser, ...globals.node } },
    rules: {
      "@typescript-eslint/no-unused-vars": ["error", { argsIgnorePattern: "^_", varsIgnorePattern: "^_" }],
    },
  },
  {
    files: ["**/*.svelte", "**/*.svelte.ts"],
    languageOptions: { parserOptions: { parser: ts.parser, extraFileExtensions: [".svelte"] } },
  },
);
