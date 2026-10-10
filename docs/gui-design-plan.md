# GUI dizajn i plan — EvolutionaryNeuralAgents (ImGui)

Status: brainstorm/dizajn (oktobar 2026). Dopuna dokumenta `ui-ux-design.md`; konkretizuje layout, panele i redosled implementacije. Mockup slike (gui_main.png, gui_inspector.png) su poslate u konverzaciji.

## 1. Ključna opservacija o layoutu
Svet je 40×60 (worldLimit ±20 / ±30) → **portretni 2:3 viewport**. Široki prozor sa simulacijom preko celog ekrana bi bacio ~60% širine. Zato **3 kolone**:

| Kolona | Širina (na 1600×900) | Sadržaj |
|---|---|---|
| Levo | 300 px | Parametri (Experiment 🔒, GA, Fitness weights, Display) |
| Centar | ostatak | Viewport sa simulacijom + toolbar overlay-a |
| Desno | 540 px | Tabovi: Statistics / Agent Inspector / Hall of Fame / Compare runs |

Gore: menu bar + transport traka. Dole: status bar (FPS, vreme sim/render/ui, autosave). Prozor povećati sa 800×600 na min. ~1600×900 (resizable).

## 2. Paneli

### Transport traka
Reset · Play/Pause · Step · Next gen · Turbo · slider brzine (+ preseti 1×, MAX) · progress bar generacije i koraka epizode · brojači alive/reached/crashed/timeout (boje = semantika, iste svuda).

### Viewport (centar)
- Postojeći `Renderer`, ali sa `sf::View` viewport-om ograničenim na centralni region (`letterboxViewport` već postoji).
- Overlay toggle-ovi: sensor rays, best trail, death heatmap, occupancy, grid; "Color by": status / fitness / heading.
- Interakcija: scroll = zoom, MMB = pan, klik = selekcija agenta (pixel → world preko `mapPixelToCoords`), dvoklik/F = follow, hover tooltip.
- HUD (gen, korak, best agent) + legenda. Klik ne prolazi kroz ImGui prozore (`io.WantCaptureMouse`).

### Statistics tab
- KPI kartice: best fitness, reach rate, mean ± σ, vreme generacije/ETA (sa trendom).
- Grafici (ImPlot): (1) fitness best/mean±σ/median/worst, (2) outcomes stacked area reach/crash/timeout, (3) fitness breakdown najbolje jedinke (stacked), (4) GA parametri kroz vreme (relevantno zbog `ParamController`).
- Hover crosshair sinhronizovan između grafika, klik na grafik = skok na tu generaciju u Hall of Fame, export CSV/PNG.
- Svi izvori podataka: `Statistics` / `GenerationStatistics` (već gotovo — jedan izvor istine za konzolu i UI).

### Agent Inspector tab
Live fitness breakdown izabranog agenta, polarni prikaz 7 senzora, izlazi mreže (turn/speed), **NN dijagram 11→10→4→2 sa live aktivacijama** i težinama (+plavo/−crveno), "Follow", "Replay genome", "Pin as reference", export genoma u JSON.

### Hall of Fame tab
Najbolji genom po generaciji (tabela sortabilna po fitnessu/gen), klik = replay u viewport-u. Veza sa `best_genome.json` iz roadmap-a (Faza 7.6).

### Compare runs tab (kasnije)
Učitavanje više `experiments/*/statistics.csv`, overlay krivih, mean ± std preko seedova — direktno služi Fazi 5 (baseline vs evolved, multi-seed, ablation).

### Parametri (levo)
- **Experiment** (seed, agents, max steps, generations): zaključano (🔒) dok ne uradiš Reset — mogu da se menjaju samo pri pokretanju.
- **GA** (elitism, mutation ratio/strength, tournament) i **Fitness weights**: live-tunable. Izmenjena polja se obeleže (narandžasto), baner "N changes pending — take effect at gen X" + Apply now / Revert. Promene se primenjuju **samo na granici generacije**.
- Kad je Adaptive (ParamController) uključen, slajderi prikazuju trenutne adaptirane vrednosti kao read-only.
- Display: visualize every N, from gen, hidden speed.
- Save/Load config (`ExperimentConfig` JSON već postoji) + Presets.

## 3. Dodatni predlozi (preko 4 tražene stavke)
1. Agent Inspector sa NN vizualizacijom (najveća portfolio vrednost + realan debugging alat).
2. Hall of Fame + replay najboljeg genoma.
3. Death heatmap / occupancy overlay — pokazuje GDE agenti umiru, dijagnostika bolja od brojeva.
4. Turbo mod (trening bez crtanja, UI osvežava grafike ~5×/s) sa budžetom ms-po-frejmu da UI ostane responzivan.
5. Compare runs (multi-seed overlay) — veza sa naučnom analizom iz roadmap-a.
6. Tooltips na svakom parametru (šta radi, tipičan opseg), pending/locked indikatori.
7. Kasnije: map editor (prevuci prepreke/cilj/spawn) za testiranje generalizacije (train na A, test na B); screenshot/GIF snimanje za README.
8. Tastaturne prečice (Space, →, F, R, T) + overlay sa pregledom prečica.

## 4. Vizuelni jezik
Tamna tema, jedan akcenat (plava), semantičke boje konzistentne u svim panelima i grafikama: reached = zelena, crashed = crvena, timeout = žuta/amber, best/elite = zlatna, selected = beli prsten. Pending izmene = narandžasto. Font: TTF (Inter/Roboto) učitan preko `io.Fonts->AddFontFromFileTTF` umesto default ProggyClean; ImGui `Style` podešen jednom u `UILayer::applyTheme()`.

## 5. Tehnička arhitektura
- `gui/UILayer` (već prazan skeleton) vlasnik ImGui-SFML init/shutdown, teme i `UIState` (selektovani agent, overlay flagovi, aktivni tab, camera).
- Paneli kao zasebne klase/funkcije: `TransportPanel`, `ParamsPanel`, `StatsPanel`, `InspectorPanel`, `HallOfFamePanel` (flat u `gui/`).
- **Tok podataka jednosmeran**: domain → UI read-only (const accessor-i, `Statistics`), UI → `UICommands`/`RuntimeConfig` (pending izmene) → domain čita na granici generacije. `GAParams` je već plain struct i prirodna osnova za RuntimeConfig.
- Zavisnosti koje fale u CMake-u: **ImPlot** (FetchContent, verzija kompatibilna sa imgui v1.91.9). Napomena: imgui je trenutno `v1.91.9` mainline; **docking** je zaseban branch — za v1 fiksan layout (prozori sa fiksnim pozicijama/flagovima) je dovoljan, docking kasnije.
- `Renderer` treba mali dodatak: render u zadati viewport rect + `screenToWorld()` za picking.
- Turbo: `Trainer::step` u petlji sa vremenskim budžetom po frejmu, bez `drawFrame()`.

## 6. Redosled implementacije (rizik prvo)
1. ImGui-SFML init u `Program` + `UILayer` skeleton + tema/font + 3-kolonski layout sa praznim panelima; viewport u centralni region.
2. Transport traka (vezana na `FixedTimestep`; fix za `visibleSpeedMultiplier`).
3. Statistics tab: KPI + 3 grafika (ImPlot) iz `Statistics`.
4. Picking/selekcija agenta + overlay toggle-ovi + zoom/pan.
5. Parametri panel + RuntimeConfig + pending/apply mehanizam (najveći refaktor).
6. Agent Inspector (senzori, izlazi, NN dijagram).
7. Hall of Fame + replay; Compare runs; heatmap; map editor.

## 7. Otvorena pitanja
- Šta tačno radi "Reset": novi random genomi ili zadržavanje elite kao seed-a?
- Da li Adaptive (ParamController) treba da bude izbor u UI-ju ili samo flag pri pokretanju?
- Da li prozor treba da podrži fullscreen/veći viewport mod ("Zen mode": samo simulacija bez panela)?
