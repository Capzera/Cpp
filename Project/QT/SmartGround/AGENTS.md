# SmartGround project instructions

For every task in this repository, treat `.cursorrules` and `Cursor_Workflow.md` as the project requirements. Read `docs/ProjectSummary.md`, `docs/ModuleStatus.md`, `docs/outlines.md`, and `docs/BugLog.md` before substantial implementation work. Keep these documents synchronized with code changes.

## Required collaboration and implementation rules

- Before implementing a new algorithm module, present its animation storyboard, including pointers, temporary visual structures, labels, and state colors. Wait for the user to approve the storyboard before writing that module's C++ implementation.
- Do not add code comments. Keep four-space indentation and readable, compact C++17 formatting.
- Keep one algorithm concept per module and derive every module from `IModule`.
- Use the base class `IModule::m_scene`; never redeclare `m_scene` in a module subclass.
- Honor `m_animated`: when false, update scene state immediately and do not create or start Qt property animations.
- Reuse the animation helpers in `IModule` for shared effects. Add a common helper before duplicating a new reusable effect in modules.
- Follow the OI/CSP-J/S teaching style and the visual semantics in `Cursor_Workflow.md`.
- Keep algorithm and rendering separated through recorded `AnimFrame` sequences. Ensure build, undo, and replay start from the correct input and scene state.
- Do not print complete source files in replies. Name changed files and functions, summarize the changes, and report build or verification results.
- Update `docs/BugLog.md`, `docs/ModuleStatus.md`, and `docs/ProjectSummary.md` when a bug is fixed or module status changes.

## Collaboration mode: PM authorization and batched execution

- The user (Rodriguez) acts as project manager and reviews only the final result. The agent owns the technical decisions and executes the full change set.
- Run the whole pipeline autonomously in one pass: evaluate, implement, compile, test, and sync docs. Never pause mid-task to ask for permission.
- Escalate only for external third-party dependencies (write a proposal first), irreconcilable conflicting requirements, or destructive operations on user data.
- Batch related requests into grouped clusters (refactor, visual/UI, interaction/animation, docs) and land them together.
- Prefer modifying existing source over adding external libraries.
- Keep the UI dependency-free and fully custom-drawn via `src/ui/widgets/` plus the `src/ui/UiTheme.h` design tokens; do not reintroduce native Qt input widgets.

## Canvas and layout conventions

- Center and zoom the viewport through `src/core/ViewportFitter.h` instead of hand-coded transforms.
- Manage invisible alignment snapping through `src/render/SceneGrid.h` with magnetic node alignment, cell snapping, and `Ctrl` for free dragging.
- Register graph edges with `node->addEdge(edge)` to prevent canvas tearing while dragging nodes.
