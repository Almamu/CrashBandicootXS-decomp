// No console window on Windows in a release build.
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

fn main() {
    std::process::exit(crash_editor_lib::run());
}
