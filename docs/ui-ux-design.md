# UI/UX brainstorm — EvolutionaryNeuralAgents aplikacija

Cilj: podići aplikaciju sa "SFML prozor + printf tabela u konzoli" na kvalitetan interaktivni alat za posmatranje, kontrolu i inspekciju treninga. Ovo je dizajn dokument (ideje/opis) — implementaciju radi Marko u CLion-u.

## Preporuka za UI stek
Dear ImGui + imgui-sfml binding za kontrolne panele, ImPlot (companion biblioteka) za live grafike. Standardna kombinacija u game-dev/simulation tooling svetu za tačno ovaj slučaj (viewport + kontrolni paneli). Alternativa (ručno pisan SFML UI: dugmad/slajderi/grafici od nule) je mnogo veći, niskoprioritetan inženjerski poduhvat koji ne dodaje vrednost ML/algoritamskom delu projekta.

## Layout
Podeljen prozor: simulacioni viewport (postojeći Renderer) + bočni/donji panel sa sekcijama (tabovi ili sekcije): Transport controls, Statistika, Parametri, Agent Inspector. Docking (resize-ovanje panela) je "nice to have" za kasnije, ne za v1 — fiksan layout je dovoljan za početak.

## Paneli

### 1. Transport controls (Start/Pause/Step/Speed)
- Play/Pause dugme (postoji kao Space, dodati i dugme), Step (postoji kao Right), "Skip to next generation" (koristi postojeći hiddenSpeedMultiplier), "Reset simulation" (jasno definisati šta reset znači — nov random genom ili zadržati trenutnu elitu kao seed).
- Slajder za speedMultiplier umesto/pored tastature.
- Status: broj generacije, korak unutar generacije, reach/crash/tout tekući brojač za generaciju u toku.

### 2. Statistika panel
- Live linijski grafik best/mean/median fitness kroz generacije (ImPlot) — najveća vrednost, trenutno zakopano u konzolnom tekstu.
- Grafik reach/crash/tout kroz generacije — vizuelno pokazuje tranziciju koju smo ručno očitavali iz tabele.
- Fitness breakdown najbolje jedinke (prog/surv/goal/collision/speed) kroz generacije — vizuelizuje tenziju između reward termina (npr. surv vs speed).
- Arhitekturna napomena: `Simulation::printGenerationStats()` trenutno računa i ispisuje u istoj funkciji. Razdvojiti "compute" (vraća `GenerationStats` value struct) od "consume" (i printf logger i UI panel čitaju isti struct) — jedan izvor istine, bez duplirane logike.

### 3. Parametri panel
- Slajderi/input polja za live-tunable parametre: mutationRatio, mutationStrength, elitismRatio, tournamentSize, FitnessWeights::*, hiddenSpeedMultiplier, visualizeEveryN/FromGeneration.
- **Preduslov (veći refaktor)**: Params su trenutno `inline constexpr` — treba runtime-mutable `RuntimeConfig` struct, prosleđen po referenci u GeneticAlgorithm/Mutation/Selection/Trainer/Simulation, umesto direktnog čitanja `Params::x`. Strukturne konstante (network topology, world size, window size) ostaju `constexpr` — jasno razdvojiti "live-tunable" od "fiksno pri pokretanju" i u UI (npr. disabled/read-only prikaz za ono što se ne sme menjati u letu).
- Promene FitnessWeights uzimati u obzir tek na granici generacije, ne usred epizode (radi konzistentnosti elitizma).

### 4. Agent Inspector (bonus, portfolio-vredno)
- Klik na agenta u mapi → panel sa: fitness breakdown do sada, 7 očitavanja senzora, trenutni izlaz mreže (turn, speed).
- Pretvara simulaciju iz "oblaka tačaka" u nešto što se stvarno može debug-ovati/inspektovati.

### 5. Kontrole mape
- Zoom/pan (sf::View).
- Toggle overlay-a: sensor rays (postoji), trail za SELEKTOVANOG agenta (ne za sve — cena performansi/vizuelni haos na 3000 agenata).
- Vizuelno istaknuta trenutno najbolja jedinka (elita).

## Arhitekturni principi za UI sloj
- UI sloj čita iz domenskih klasa (Simulation/GeneticAlgorithm/Population) preko postojećih const accessor-a — read-only. Pisanje parametara ide isključivo kroz `RuntimeConfig`, ne direktnom mutacijom polja — jednosmeran tok podataka (domain → UI read-only, UI → RuntimeConfig write, domain čita RuntimeConfig na granici generacije).
- ImGui + SFML rade na glavnoj niti (immediate-mode) — nema threading komplikacija.
- "Reset" treba biti eksplicitna, dobro definisana operacija na Program/Trainer nivou, ne poziv konstruktora iznova.

## Organizacija fajlova
Nov `gui/` folder, paralelno sa `app/`, `world/`, `neural/`... Krenuti flat (`gui/UILayer.h/.cpp` + po jedan fajl za prvi panel), bez unapred pravljenog `gui/panels/` podfoldera — isti obrazac kao `world/obstacles/` koji je nastao tek kad su postojala 2 tipa prepreka, ne unapred.

## Predlog redosleda rada (revidirano)
Originalni redosled je stavljao RuntimeConfig refaktor prvi jer je "preduslov za sve ostalo" — tačno za Parametri panel, ali ne i za Transport controls, koji može da radi nad postojećim mutable `FixedTimestep` objektom (`togglePause()`, `requestSingleStep()`, `setSpeedMultiplier()`) koji `Program` već poseduje, bez ikakve arhitekturne promene. Revidiran redosled je risk-first: najrizičniji/najmanje poznat deo (integracija third-party biblioteke u CMake) se dokazuje prvi, pre nego što se refaktori grade na neproverenom toolchain-u, i daje brz, konkretan, vidljiv rani rezultat.

1. ImGui/imgui-sfml CMake integracija + minimalan Transport Controls panel (Play/Pause/Step/Speed vezani direktno na postojeći `FixedTimestep`) — dokazuje toolchain, nulta promena arhitekture, brza pobeda.
2. RuntimeConfig refaktor (preduslov za Parametri panel i dalje).
3. GenerationStats extract (preduslov za Statistika panel).
4. Statistika panel (ImPlot).
5. Parametri panel.
6. Agent Inspector (bonus).
