# EvolutionaryNeuralAgents — Roadmap / Backlog

## Trenutno stanje (septembar 2026)
- Genetski algoritam (Selection/tournament, Crossover uniform+blend, Mutation, elitism) implementiran i empirijski potvrđen kroz stvarne treninge, populacija testirana do 3000 agenata.
- Fitness funkcija: progress + survival + goal + collision + speed-to-goal bonus.
- Senzori: analitički ray-cast (ray-circle, ray-rect slab test), World::boundaryDistance popravljen.
- `statistics/` modul (GenerationStatistics, Calculation, Storage, Export, Statistics) implementiran i ukačen u Simulation/Trainer (`recordGenerationStats()` + `printGenerationStats()` čita iz Statistics). Nekompajlirano od strane asistenta (nema build alata u sandbox-u) — potrebno je da Marko potvrdi build u CLion-u.
- `speedMultiplier` bug dijagnostikovan i rešen: `Program::updateVisualizationPolicy()` je hardkodovao `1.0` za vidljive generacije umesto podesivog `Params::visibleSpeedMultiplier` — fix opisan, čeka implementaciju.
- Dijagnostikovan problem: mutation-selection erozija populacije posle konvergencije (reach pada sa ~83/100 na ~25-40/100 posle gen ~130 na fiksnom mutationRatio/mutationStrength=0.20).
- Arhitekturna provera (bitna za redosled ispod): `Simulation::update()`/`Trainer::step()` ne zavise od SFML prozora — `Renderer`-ov konstruktor samo čuva reference, `render()` se zove isključivo iz `Program::drawFrame()`. Jedino mesto gde se "SFML zarazi" ceo program je `Program`-ov konstruktor (pravi `sf::RenderWindow` bezuslovno) i `main.cpp`. Headless mod je zato jeftin — novi entry point, ne arhitekturni refaktor.

## Cilj: od "dobrog studentskog projekta" do research-grade portfolio projekta

Marko je (uz pomoć eksternog pregleda) identifikovao da je najveća slabost projekta trenutno **nedostatak dokaza** — testiranje, eksperimentalna rigoroznost, dokumentacija, reprodukovanje — a ne nedostatak feature-a. Arhitektura (world/sensors/neural/evolution/fitness/simulation/statistics/app/gui/rendering, jasna separation of concerns) je već dovoljno dobra da se na tome gradi bez prepravki.

## Fazni plan (revidiran redosled — zasnovan na proveri koda, ne samo na inicijalnom predlogu)

### Faza 1 — Headless mod (jeftino, visok leverage, radi se prvo)
- Novi entry point (ili `main.cpp` flag `--headless --generations N --seed S`) koji direktno konstruiše World/Population/GeneticAlgorithm/Simulation/Trainer, zaobilazeći `Program` i `sf::RenderWindow` potpuno.
- Izlaz: `statistics.exportToCSV(...)` na kraju runa (ili periodično).
- Otključava: CI (bez potrebe za virtuelnim displejom), batch eksperimente (Faza 3), i ne zavisi ni od čega drugog u ovoj listi.

### Faza 2 — Testing (može paralelno sa Fazom 1, nema blokirajuću zavisnost)
- `Vec2`, `Matrix`, `Calculation`, `Fitness`, `Selection`/`Crossover`/`Mutation` su već potpuno odvojeni od SFML-a — testabilni odmah, bez čekanja na headless mod.
- Frejmvork: Catch2 ili GoogleTest (FetchContent) — Marko bira.
- Prioritet: invarijante (population.size() konstantan kroz GA korak, genome.size() == parameter count mreže, eliteCount <= populationSize) i property-based provere (A×I=A za Matrix, normalize(v).length()≈1), ne samo primer-po-primer testovi.
- `Simulation`/`Episode` su takođe testabilni bez prozora (render() se ne zove iz update() petlje).

### Faza 3 — Experiment engine (gradi se na Fazi 1)
- Config (seed, population, generations, GA parametri, fitness weights) kao JSON/struct, snimljen uz svaki run.
- Folderi po eksperimentu: `experiments/<datum>_<id>/{config.json, statistics.csv, best_genome.json}`.

### Faza 4 — CI (gradi se na Fazi 1 + 2, ne čeka pun CMake target split)
- GitHub Actions: checkout → cmake → build → run tests. Dovoljno je da test binarni fajl nikad ne instancira `Program` (nema potrebe za Xvfb/virtuelnim displejom ako testovi ne prave pravi prozor).

### Faza 5 — Naučna analiza (gradi se na Fazi 3)
- Baseline (random-policy agent) vs evolucioni agent — merljiv efekat (success rate, avg progress, avg fitness).
- Multi-seed runovi (npr. 20 seedova) — mean/std final fitness, mean success rate.
- Ablation studies na FitnessWeights (bez speed, bez collision penalty, samo progress+goal).
- Hyperparameter sweep (mutation rate/strength, population size, elitism) — par pažljivo izabranih eksperimenata, ne grid search od 1000.

### Faza 6 — Optimizacija performansi (postaje bitna kad Faza 5 pokreće puno runova)
- Persistent/reused Matrix baferi (Layer::forward, NeuralNetwork::forward, Agent::scanEnvironment/encodeInputs) — izbeći heap alokaciju po koraku po agentu.
- Paralelizacija po agentima u Simulation::update().

### Faza 7 — GUI (tek kad prethodno postoji — tad ima šta da prikaže)
1. ImGui/imgui-sfml CMake integracija + minimalan Transport Controls panel (Play/Pause/Step/Speed vezan na `FixedTimestep`, uključujući `Params::visibleSpeedMultiplier` fix).
2. RuntimeConfig refaktor (live-tunable parametri umesto `inline constexpr`).
3. GenerationStats extract — GOTOVO (statistics/ modul).
4. Statistika panel (ImPlot).
5. Parametri panel.
6. Agent Inspector + Replay najboljeg genoma (best_genome.json → NeuralNetwork → Agent → replay).

### Faza 8 — Portfolio polish
- README (arhitektura, kako se pokreće, rezultati, metodologija eksperimenata), CI badge, GIF/video, build instrukcije.

### Stretch / posle svega gore (namerno odloženo, dobre ideje ali ne sad)
- Nivo 1 adaptivna kontrola GA hiperparametara (Rechenberg 1/5 pravilo) i Nivo 2 Population Based Training (Jaderberg et al. 2017, DeepMind) — sad se prirodno uklapaju kao eksperimenti u Fazi 5, ne kao posebna faza.
- NEAT-style topology evolution (evolucija arhitekture mreže, ne samo težina) — jako istraživačko pitanje, ali tek pošto je dokazano da trenutni fixed-topology sistem radi.
- Više environment-a (open navigation / maze / uski hodnici / dinamičke prepreke) za testiranje generalizacije (train na A, test na B).

## Napomena o tempu
Ceo plan (Faze 1-8) je ambiciozan pored ETF obaveza — prioritet je leverage/cena, ne striktna linearnost kroz sve tačke. Faza 1 + početak Faze 2 su sledeći konkretan korak.
