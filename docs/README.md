# Dokumentacija

- [roadmap.md](roadmap.md) — fazni plan projekta i backlog
- [ui-ux-design.md](ui-ux-design.md) — originalni UI/UX brainstorm
- [gui-design-plan.md](gui-design-plan.md) — konkretan plan GUI-ja (layout, paneli, redosled)

Napomena: putanje fajlova u ovim dokumentima (npr. `gui/UILayer`, `statistics/`) opisuju strukturu pre
reorganizacije. Sav kod je sada u `src/`:

| Sloj | Folderi |
|---|---|
| `src/core/` | `math/`, `neural/` |
| `src/model/` | `world/` (+ `obstacles/`), `sensors/`, `simulation/` |
| `src/learning/` | `evolution/`, `fitness/`, `statistics/` |
| `src/app/` | `Program`, `Trainer`, `FixedTimestep`, `HeadlessRunner`, `config/` |
| `src/ui/` | `rendering/`, `gui/` (+ `panels/`) |
