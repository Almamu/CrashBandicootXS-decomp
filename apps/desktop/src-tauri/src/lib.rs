//! The desktop editor (#1167, docs/editors.md "The desktop app"): apps/editor's
//! build in a window, on the backend of a checkout of the decomp.
//!
//! The backend stays the one implementation for the browser and the desktop:
//! this app starts the checkout's `tools/editor_server.py` (Python 3.12+) on a
//! free port of 127.0.0.1 with a random token, and gives the page its URL and
//! token (`window.__CRASH_EDITOR__`) before the page loads. The checkout is
//! asked for on the first run (a folder dialog) and remembered in the app's
//! config directory (`config.json`); File > "Choose the checkout…" changes it.
//!
//!     crash-editor [--checkout DIR] [--python CMD]   the app
//!     crash-editor --smoke [--checkout DIR]          no window: start the backend,
//!                                                    ask its /api/health, stop (CI)

use std::collections::hash_map::RandomState;
use std::hash::{BuildHasher, Hasher};
use std::io::{BufRead, BufReader, Read, Write};
use std::net::TcpStream;
use std::path::{Path, PathBuf};
use std::process::{Child, Command, Stdio};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{mpsc, Arc, Mutex};
use std::time::Duration;

use serde::{Deserialize, Serialize};
use tauri::menu::{Menu, MenuItem, PredefinedMenuItem, Submenu};
use tauri::{AppHandle, Manager, RunEvent, WebviewUrl, WebviewWindowBuilder};
use tauri_plugin_dialog::{DialogExt, MessageDialogKind};

const START_TIMEOUT: Duration = Duration::from_secs(60);

// ---------------------------------------------------------------------------
// The command line and the remembered settings

#[derive(Default, Clone)]
struct Args {
    checkout: Option<PathBuf>,
    python: Option<String>,
    smoke: bool,
}

fn parse_args() -> Args {
    let mut out = Args::default();
    let mut it = std::env::args().skip(1);
    while let Some(a) = it.next() {
        match a.as_str() {
            "--checkout" => out.checkout = it.next().map(PathBuf::from),
            "--python" => out.python = it.next(),
            "--smoke" => out.smoke = true,
            _ => {}
        }
    }
    out
}

#[derive(Serialize, Deserialize, Default)]
struct Config {
    checkout: Option<PathBuf>,
    python: Option<String>,
}

fn config_path(app: &AppHandle) -> Option<PathBuf> {
    app.path().app_config_dir().ok().map(|d| d.join("config.json"))
}

fn load_config(app: &AppHandle) -> Config {
    config_path(app)
        .and_then(|p| std::fs::read_to_string(p).ok())
        .and_then(|t| serde_json::from_str(&t).ok())
        .unwrap_or_default()
}

fn save_config(app: &AppHandle, cfg: &Config) {
    if let Some(p) = config_path(app) {
        if let Some(d) = p.parent() {
            let _ = std::fs::create_dir_all(d);
        }
        if let Ok(t) = serde_json::to_string_pretty(cfg) {
            let _ = std::fs::write(p, t);
        }
    }
}

/// A checkout of the decomp: it has the backend.
fn is_checkout(p: &Path) -> bool {
    p.join("tools").join("editor_server.py").is_file() && p.join("packages").join("crashdata").is_dir()
}

// ---------------------------------------------------------------------------
// The backend: the checkout's tools/editor_server.py

struct Backend {
    child: Child,
    /// the backend's stdin: it stops when this closes (--watch-stdin), however the app ends
    _stdin: Option<std::process::ChildStdin>,
    url: String,
    token: String,
    checkout: PathBuf,
}

impl Drop for Backend {
    fn drop(&mut self) {
        let _ = self.child.kill();
        let _ = self.child.wait();
    }
}

fn token() -> String {
    // RandomState is seeded from the OS's random source, per instance
    (0..4u64)
        .map(|i| {
            let mut h = RandomState::new().build_hasher();
            h.write_u64(i);
            h.write_u128(std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH).map(|d| d.as_nanos()).unwrap_or(0));
            format!("{:016x}", h.finish())
        })
        .collect()
}

fn no_window(cmd: &mut Command) -> &mut Command {
    #[cfg(windows)]
    {
        use std::os::windows::process::CommandExt;
        const CREATE_NO_WINDOW: u32 = 0x0800_0000;
        cmd.creation_flags(CREATE_NO_WINDOW);
    }
    cmd
}

/// The Python to run the backend with: the one asked for, or the first of
/// the usual names that is 3.12 or later.
fn find_python(asked: Option<&str>) -> Result<Vec<String>, String> {
    let mut candidates: Vec<Vec<String>> = Vec::new();
    for s in [std::env::var("CRASH_EDITOR_PYTHON").ok(), asked.map(String::from)].into_iter().flatten() {
        candidates.push(s.split_whitespace().map(String::from).collect());
    }
    if cfg!(windows) {
        candidates.push(vec!["py".into(), "-3".into()]);
        candidates.push(vec!["python".into()]);
        candidates.push(vec!["python3".into()]);
    } else {
        candidates.push(vec!["python3".into()]);
        candidates.push(vec!["python".into()]);
    }
    let mut tried = Vec::new();
    for c in candidates.into_iter().filter(|c| !c.is_empty()) {
        let out = no_window(Command::new(&c[0]).args(&c[1..]).args(["-c", "import sys; print(sys.version_info >= (3, 12))"]))
            .stdin(Stdio::null())
            .output();
        match out {
            Ok(o) if String::from_utf8_lossy(&o.stdout).trim() == "True" => return Ok(c),
            Ok(_) => tried.push(format!("{} (older than 3.12)", c.join(" "))),
            Err(_) => tried.push(format!("{} (not found)", c.join(" "))),
        }
    }
    Err(format!(
        "no Python 3.12 or later: tried {}. Install it, or give its command with --python or CRASH_EDITOR_PYTHON.",
        tried.join(", ")
    ))
}

fn start_backend(checkout: &Path, python: &[String]) -> Result<Backend, String> {
    let token = token();
    let mut cmd = Command::new(&python[0]);
    cmd.args(&python[1..])
        .arg(Path::new("tools").join("editor_server.py"))
        .args(["--port", "0", "--watch-stdin"]);
    if std::env::var_os("CRASH_EDITOR_VERBOSE").is_some() {
        cmd.arg("--verbose");
    }
    if cfg!(debug_assertions) {
        // `tauri dev`'s page is Vite's
        cmd.args(["--allow-origin", "http://127.0.0.1:5173"]);
    }
    cmd.env("CRASH_EDITOR_TOKEN", &token)
        .env("PYTHONUNBUFFERED", "1")
        .env("PYTHONIOENCODING", "utf-8")
        .current_dir(checkout)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped());
    let mut child = no_window(&mut cmd)
        .spawn()
        .map_err(|e| format!("couldn't run {}: {e}", python.join(" ")))?;
    let (tx, rx) = mpsc::channel();
    let stdout = child.stdout.take().expect("piped");
    std::thread::spawn(move || {
        for line in BufReader::new(stdout).lines().map_while(Result::ok) {
            if let Some(u) = line.strip_prefix("editor-server: listening on ") {
                let _ = tx.send(u.trim().trim_end_matches('/').to_string());
            }
            println!("{line}");
        }
    });
    let errors = Arc::new(Mutex::new(String::new()));
    let stderr = child.stderr.take().expect("piped");
    let errs = errors.clone();
    std::thread::spawn(move || {
        for line in BufReader::new(stderr).lines().map_while(Result::ok) {
            eprintln!("{line}");
            if let Ok(mut e) = errs.lock() {
                e.push_str(&line);
                e.push('\n');
                if e.len() > 4000 {
                    let cut = e.len() - 4000;
                    *e = e[cut..].to_string();
                }
            }
        }
    });
    match rx.recv_timeout(START_TIMEOUT) {
        Ok(url) => {
            let stdin = child.stdin.take();
            Ok(Backend { child, _stdin: stdin, url, token, checkout: checkout.to_path_buf() })
        }
        Err(_) => {
            let _ = child.kill();
            let _ = child.wait();
            std::thread::sleep(Duration::from_millis(200));
            let e = errors.lock().map(|e| e.clone()).unwrap_or_default();
            Err(format!("the backend (tools/editor_server.py) didn't start:\n{}", e.trim()))
        }
    }
}

/// GET /api/health, as the page would.
fn health(b: &Backend) -> Result<String, String> {
    let hostport = b.url.trim_start_matches("http://").to_string();
    let mut s = TcpStream::connect(&hostport).map_err(|e| e.to_string())?;
    s.set_read_timeout(Some(Duration::from_secs(10))).ok();
    write!(
        s,
        "GET /api/health HTTP/1.1\r\nHost: {hostport}\r\nAuthorization: Bearer {}\r\nConnection: close\r\n\r\n",
        b.token
    )
    .map_err(|e| e.to_string())?;
    let mut out = String::new();
    s.read_to_string(&mut out).map_err(|e| e.to_string())?;
    let status = out.lines().next().unwrap_or("").to_string();
    if !status.contains(" 200 ") {
        return Err(format!("/api/health: {status}"));
    }
    Ok(out.split("\r\n\r\n").nth(1).unwrap_or("").trim().to_string())
}

// ---------------------------------------------------------------------------
// The app

#[derive(Default)]
struct State {
    backend: Mutex<Option<Backend>>,
    windows: AtomicUsize,
    python: Mutex<Option<String>>,
}

fn error_dialog(app: &AppHandle, message: String, then: impl FnOnce(bool) + Send + 'static) {
    app.dialog()
        .message(message)
        .title("Crash Data Editor")
        .kind(MessageDialogKind::Error)
        .buttons(tauri_plugin_dialog::MessageDialogButtons::OkCancelCustom(
            "Choose another checkout".into(),
            "Quit".into(),
        ))
        .show(then);
}

/// Asks for the checkout (a folder dialog), then starts on it.
fn choose_checkout(app: AppHandle) {
    let a = app.clone();
    app.dialog()
        .file()
        .set_title("Choose your checkout of CrashBandicootXS-decomp")
        .pick_folder(move |picked| {
            let Some(path) = picked.and_then(|p| p.into_path().ok()) else {
                // cancelled: quit when nothing is open yet
                if a.state::<State>().backend.lock().map(|b| b.is_none()).unwrap_or(true) {
                    a.exit(0);
                }
                return;
            };
            if !is_checkout(&path) {
                let a2 = a.clone();
                error_dialog(
                    &a,
                    format!(
                        "{} isn't a checkout of the decomp: it has no tools/editor_server.py and packages/crashdata/.",
                        path.display()
                    ),
                    move |again| if again { choose_checkout(a2) } else { quit_unless_open(&a2) },
                );
                return;
            }
            start(a, path, true);
        });
}

fn quit_unless_open(app: &AppHandle) {
    if app.state::<State>().backend.lock().map(|b| b.is_none()).unwrap_or(true) {
        app.exit(0);
    }
}

/// Starts the backend on the checkout (away from the main thread), then opens the window on it.
fn start(app: AppHandle, checkout: PathBuf, remember: bool) {
    std::thread::spawn(move || {
        let asked = app.state::<State>().python.lock().ok().and_then(|p| p.clone());
        let result = find_python(asked.as_deref()).and_then(|py| start_backend(&checkout, &py));
        match result {
            Ok(backend) => {
                if remember {
                    let mut cfg = load_config(&app);
                    cfg.checkout = Some(checkout.clone());
                    save_config(&app, &cfg);
                }
                if let Err(e) = open_window(&app, backend) {
                    eprintln!("crash-editor: {e}");
                    app.exit(1);
                }
            }
            Err(e) => {
                let a = app.clone();
                error_dialog(
                    &app,
                    format!("Couldn't start the editor's backend in {}:\n\n{e}", checkout.display()),
                    move |again| if again { choose_checkout(a) } else { quit_unless_open(&a) },
                );
            }
        }
    });
}

fn open_window(app: &AppHandle, backend: Backend) -> tauri::Result<()> {
    let state = app.state::<State>();
    let config = serde_json::json!({
        "url": backend.url,
        "token": backend.token,
        "checkout": backend.checkout.display().to_string(),
    });
    let title = format!("Crash Data Editor: {}", backend.checkout.display());
    let n = state.windows.fetch_add(1, Ordering::SeqCst);
    let label = format!("main{n}");
    let old: Vec<_> = app.webview_windows().into_values().collect();
    WebviewWindowBuilder::new(app, &label, WebviewUrl::App("index.html".into()))
        .title(title)
        .inner_size(1400.0, 900.0)
        .min_inner_size(800.0, 500.0)
        .initialization_script(format!("window.__CRASH_EDITOR__ = {config};"))
        .build()?;
    // the old window and backend go once the new ones are up
    for w in old {
        let _ = w.destroy();
    }
    if let Ok(mut b) = state.backend.lock() {
        *b = Some(backend);
    }
    Ok(())
}

fn menu(app: &AppHandle) -> tauri::Result<Menu<tauri::Wry>> {
    let choose = MenuItem::with_id(app, "checkout", "Choose the checkout…", true, None::<&str>)?;
    let reload = MenuItem::with_id(app, "reload", "Reload", true, Some("CmdOrCtrl+R"))?;
    let quit = PredefinedMenuItem::quit(app, Some("Quit"))?;
    let sep = PredefinedMenuItem::separator(app)?;
    let file = Submenu::with_items(app, "File", true, &[&choose, &reload, &sep, &quit])?;
    let edit = Submenu::with_items(
        app,
        "Edit",
        true,
        &[
            &PredefinedMenuItem::undo(app, None)?,
            &PredefinedMenuItem::redo(app, None)?,
            &PredefinedMenuItem::separator(app)?,
            &PredefinedMenuItem::cut(app, None)?,
            &PredefinedMenuItem::copy(app, None)?,
            &PredefinedMenuItem::paste(app, None)?,
            &PredefinedMenuItem::select_all(app, None)?,
        ],
    )?;
    Menu::with_items(app, &[&file, &edit])
}

fn smoke(args: &Args) -> i32 {
    let checkout = args
        .checkout
        .clone()
        .or_else(|| std::env::current_dir().ok())
        .unwrap_or_default();
    if !is_checkout(&checkout) {
        eprintln!("crash-editor --smoke: {} isn't a checkout (no tools/editor_server.py)", checkout.display());
        return 2;
    }
    let result = find_python(args.python.as_deref()).and_then(|py| {
        println!("crash-editor: python: {}", py.join(" "));
        let b = start_backend(&checkout, &py)?;
        println!("crash-editor: backend at {}", b.url);
        health(&b)
    });
    match result {
        Ok(body) => {
            println!("crash-editor: /api/health: {body}");
            0
        }
        Err(e) => {
            eprintln!("crash-editor --smoke: {e}");
            1
        }
    }
}

pub fn run() -> i32 {
    let args = parse_args();
    if args.smoke {
        return smoke(&args);
    }
    let app = tauri::Builder::default()
        .plugin(tauri_plugin_dialog::init())
        .manage(State::default())
        .setup(move |app| {
            let handle = app.handle().clone();
            handle.set_menu(menu(&handle)?)?;
            let cfg = load_config(&handle);
            if let Ok(mut p) = handle.state::<State>().python.lock() {
                *p = args.python.clone().or(cfg.python.clone());
            }
            match args.checkout.clone().or(cfg.checkout.clone()) {
                Some(c) if is_checkout(&c) => start(handle, c, args.checkout.is_some()),
                _ => choose_checkout(handle),
            }
            Ok(())
        })
        .on_menu_event(|app, event| match event.id().as_ref() {
            "checkout" => choose_checkout(app.clone()),
            "reload" => {
                for w in app.webview_windows().values() {
                    let _ = w.eval("location.reload()");
                }
            }
            _ => {}
        })
        .build(tauri::generate_context!())
        .expect("the app");
    app.run(|app, event| match event {
        // no window yet (the folder dialog, the backend starting) isn't the end
        RunEvent::ExitRequested { api, code: None, .. } => {
            if app.state::<State>().backend.lock().map(|b| b.is_none()).unwrap_or(false) {
                api.prevent_exit();
            }
        }
        RunEvent::Exit => {
            if let Ok(mut b) = app.state::<State>().backend.lock() {
                b.take();
            }
        }
        _ => {}
    });
    0
}
