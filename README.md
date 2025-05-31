# Kaufman

## 🎬 Screenplay Command-Line Editor (Alpha)

> 🚧 **Work In Progress:**  
> Kaufman is currently in experimental/testing stages. Many features are still being developed, and breaking changes are likely.  
>  
> **Not production-ready** — your feedback and contributions are welcome!

---

### About

**Kaufman** is a sleek, modern command-line text editor crafted for writing and editing screenplays. Inspired by the speed and power of `vim`, Kaufman brings modal editing and efficient workflows to the terminal — so you can focus on your story, not your tools.

---

## 🖥️ Tech Stack

- **Language:** C++
- **Terminal UI:** [ncurses](https://invisible-island.net/ncurses/) for fast, portable, and responsive interface
- **Architecture:** Clean separation of editor, buffer, and state management

---

## ✨ What Makes Kaufman Great

- **Vim-style Motions:**  
  Navigate with `h`, `j`, `k`, `l`, words, lines, and more—speed and familiarity for power-users.
- **Modal Editing:**  
  Multiple modes: `normal`, `edit` (insert), and `visual` for a focused, distraction-free writing experience.
- **Screenplay-First Workflow:**  
  Tailored navigation, formatting, and shortcuts for screenplay structure (scenes, dialogue, action, etc.) *(Planned)*.
- **Separation of Concerns:**  
  Editor and buffer are modular, paving the way for easy future enhancements and bug isolation.
- **Robust Error Handling:**  
  Early adoption of error testing and edge-case management for safe editing.
- **Modern, Minimal UI:**  
  Fast startup, clean interface, and thoughtful details for an enjoyable command-line experience.
- **Open & Extensible:**  
  Foundations for custom keybindings, theming, and plugin support.

---

## 🚀 Vision

- Terminal-based screenplay writing as fluid as prose editing in `vim`
- Professional screenplay formatting and export options
- Customizable keybindings, themes, and extensibility
- Friendly, collaborative community

---

## 🏆 Project Highlights

- **Modern C++ codebase** for performance and maintainability
- **Thoughtful separation** between editor and buffer logic
- **Careful error handling** and early test development
- **Leverages ncurses** for a cross-platform, efficient terminal UI
- **Open to collaboration, feedback, and future extensibility**

---

## 📦 Getting Started

1. **Clone the repository:**
   ```sh
   git clone https://github.com/phagmaier/Kaufman.git
   ```
2. **Install dependencies:**  
   Make sure you have `ncurses` installed on your system.  
   - **Debian/Ubuntu:** `sudo apt-get install libncurses5-dev libncursesw5-dev`
   - **macOS (with Homebrew):** `brew install ncurses`
   - **Fedora:** `sudo dnf install ncurses-devel`
3. **Build the project:**
   ```sh
   cd Kaufman
   make
   ```
4. **Run the editor:**
   ```sh
   ./kaufman
   ```
5. **Try it out:**
   - `i` to enter edit mode
   - `Esc` to return to normal mode
   - `v` to enter visual mode
   - `:q` to quit

> See the [`docs/`](docs/) folder for more details and tips.

---

## 🤝 Contributing

Kaufman is in its early days and **needs your feedback!**  
Bug reports, feature requests, and pull requests are welcome.

---

## 📄 License

**Kaufman** by [phagmaier](https://github.com/phagmaier) — MIT License

[GitHub: phagmaier/Kaufman](https://github.com/phagmaier/Kaufman)

---

<sub>Made with ❤️ in C++ and ncurses.</sub>
