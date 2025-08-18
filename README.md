# Eugenio's technical test notes

First of all, I want to thank Geotech for this opportunity. In good conscience, I truly gave it my best.

Here is a breakdown in sections of how I invested my time, the core architecture and some of the design principles behind my choices.

---

## Timeline

To achieve this project, I invested a total of **3 and a half days**.
I spent my effort around the 5-day delivery threshold. The goals and expectations were set based on a compromise between ambition and reality: I wanted to deliver in good time, but also didn't want sloppy work.

* **Day 0 → 0.5**: The goal was to understand what I was going to build, and how to build it. Project setup, quick survey of patterns, case seudies, macro architectural choices.
* **Day 0.5 → 1.5**: The goal was to achieve a working prototype in-editor before the end of day 2: core logic, grid interaction, basic view; rapid code–fix cycles, achieved at the cost of tech debt.
* **Day 1.5 → 2.5**: The goal was to know I was in safe waters before the end of day 3: architecture consolidation, missing features closed, some polish passes on UX and behavior.
* **Day 2.5 → 3.5**: The goal was to deliver. Naming/cleanup, comments, settings pass, final review, writing this. Also took a healthy break to avoid delivering on a cluttered mind.

The most intense days were definitely the ones in the middle, but once I knew all the work left was refactoring, commenting, and some polishing, the development experience was smooth sailing.

---

## Design Principles

* **Less code is better code.** Prefer built-in engine behaviors and Slate widgets over custom plumbing. Avoid premature future-proofing when it adds cost without present value.
* **YAGNI (you aren’t gonna need it).** - extented to "If we don’t *know* it’s needed, we don’t build it". As long as today’s requirements are met and we don't incur technical debt, the best kind of future-proofing is saved effort.
* **No duplication.** Don’t create proxy layers that only forward calls; keep the project small, easy to navigate and review.

---

## Core architecture breakdown

### High level

The project is intentionally small: A **Model** (`FSlateSweeperGameModel`) that owns the minefield and rules, a **Controller** (`FSlateSweeperGameController`) that translates user intent into state mutations, and a **View** (`SSlateSweeperGameView`) that renders and relays input.

The editor tab (`SSlateSweeperTab`) simply hosts the View and settings menu (`SSlateSweeperMenu`); the game logic doesn’t depend on the tab’s lifetime. This way you can open/close tab and potentially render the view anywhere.

If we think in design pattern terms, this architecture is a bit of a stricter MVC where the controller is designed to own model and view, and to be the sole point of contact for the external world. for the scope of this project, I actually ended up exposing the `Model` and the `View` (via `Controller` functions) for simplicity.

Some helper files tie things together:
* `SlateSweeperTypes.h` is our key player in ensuring everyone agrees on how data looks like
* `SlateSweeperEditorSettings.h` is a quality-of-life class that contains the game setup criteria (used in the menu), while also storing settings data which made testing much faster.

### Data and compute/memory costs

The grid state is centralized in `FSlateSweeperGridData`, which stores:

* `RevealedCells` → a `TBitArray<>` marking which tiles are visible.

* `CellNeighbourCounts` → a `TArray<uint8>` representing the mine count around each cell. Cells without neighbours are still present as array elements to keep access at O(1).

* `MineCells` → a `TBitArray<>` marking mine positions.

* `GridWidth` / `GridHeight` → dimensions of the current session.

This struct acts as the immutable “truth” the View renders from, while the `Model` is responsible for mutating it safely.

The grid initialization is done at `model` construction which calls `AllocateMines`, `ComputeMineNeighbourCounts`.

While this creates more data than needed (an array element in every array for every cell) it keeps code simple and operations trivial.

For reference, a 255x255 grid will take ~81kb, counting 65,025 cells. The `View` is orders of magnitude heavier, so we'd start optimisation there!

### Event flow

**Starting a game:**
* Get `FSlateSweeperEditor` (module)
* Call `StartNewGame(const FSlateSweeperNewGameSettings& GameSettings)`
* The module spawns a controller which creates a model which creates the data
* Grab a reference to the view from the controller (which will spawn the view if doesn't exist yet)

**During game:**
* View raises `OnCellPressed(int32 Index)`
* Controller calls State, which responds with the result, and controller triggers a **single view redraw**.
* View renders from data only.

This structure keeps the project size tiny, ready to future extension without paying for them now.
For example, the view could potentially switch between different games, even resizing itself, as it treats every state as an immutable source of truth.

---

## To `int` or not to `int`

A couple words on Unreal and the type `int`. Some years ago, I used to go for the perfect type at every step of the way.
Today, my opinion on the matter is that many times in Unreal you’re just masking implicit conversion which is the worse tradeoff.

**`int32` is the Unreal lingua franca.** It minimizes friction with engine APIs and implicit conversions. Sometimes it likes `uint8`.

That being said, I worked in companies that used `size_t` and so forth, and was happy to abide the coding standards.

---

## AI usage

### _AI is fantastic when it works, frustrating when it fails, dangerous when it pretends._

I used AI mostly in three ways:

* A support tool in the early stages, mainly to accelerate routine tasks and keep momentum while aiming for a working prototype. For example, I offloaded the first draft of neighbor computation and cascade reveal logic. I later rewrote and polished all code.
* Brainstorming: explore possible class names, patterns, and best practices, which I then validated through my own research.
* Any routine task that has a clear, predictable result (e.g. creating a bunch of developer settings, although I still had to make some final touches)

Two examples of how I interacted with AI:

* [Architecture-focused discussion](https://chatgpt.com/share/68a1ed98-3640-800f-93f1-038989e078a5) (walk & talk turned into pragmatic schematics/suggestions)
* [Code-and-fix focused discussion](https://chatgpt.com/share/68a1edc7-569c-800f-9a92-4e1d132b878c) (predictable tasks, or "let's give AI a shot" cases)
* If I didn't like the output, I just ignored it and moved on.

In the final stages, my use of AI was very limited. The main issue is accountability: AI can act like a senior developer but does not grow like a junior. Its mistakes are often subtle and repetitive, hidden behind confident explanations. When this happens, debugging becomes time-consuming.

All in all, having a second perspective helped me a lot in this solo endeavour.

---

## Closing

This project intentionally optimizes for **clarity, and tiny surface area** over speculative flexibility. If I would do it again, I would probably change a lot of things, but it's easy to say in hindsight.

Last but not least, a sincere thank you to the reviewers. I approached this test with genuine dedication, and I simply hope it can be evaluated with fairness.


Cheers!

Eugenio
