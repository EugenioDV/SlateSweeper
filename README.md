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

### Event flow

* View raises `OnCellPressed(int32 Index)`
* Controller validates, mutates State, and triggers a **single redraw**.
* View renders from Data only.

This structure keeps the project size tiny, ready to future extension without paying for them now.
For example, the view could potentially switch between different games, even resizing itself, as it treats every state as an immutable source of truth.

---

## To `int` or not to `int`

A couple words on Unreal and the type `int`. Some years ago, I used to go for the perfect type at every step of the way.
Today, my opinion on the matter is that many times in Unreal you’re just masking implicit conversion which is a worse tradeoff than using the perfect type.

**`int32` is the Unreal lingua franca.** It minimizes friction with engine APIs and implicit conversions. Sometimes it likes `uint8`.

That being said, I worked in companies that used `size_t` and so forth, and was happy to abide the coding standards.

---

## AI is fantastic when it works, frustrating when it doesn't, dangerous when it pretends.

I used AI mostly in two ways:

* A support tool in the early stages, mainly to accelerate routine tasks and keep momentum while aiming for a working prototype. For example, I offloaded the first draft of neighbor computation and cascade reveal logic. I later rewrote and polished all code.
* Brainstorming: explore possible class names, patterns, and best practices, which I then validated through my own research.

Two examples of how I interacted with AI:

* [Architecture-focused discussion](https://chatgpt.com/share/68a1ed98-3640-800f-93f1-038989e078a5)
* [Code-and-fix focused discussion](https://chatgpt.com/share/68a1edc7-569c-800f-9a92-4e1d132b878c)

In the final stages, my use of AI was very limited. The main issue is accountability: AI can act like a senior developer but does not grow like a junior. Its mistakes are often subtle and repetitive, hidden behind confident explanations. When this happens, debugging becomes time-consuming.

That said, having a second perspective, even if not human, helped me a lot in this solo endeavour.

---

## Closing

This project intentionally optimizes for **clarity, and tiny surface area** over speculative flexibility. It ships a clean baseline and leaves obvious seams for future growth without front-loading that cost.

That being said, if I would do it again, I would probably change a lot of things, but it's easy to say in hindsight.

Last but not least, a sincere thank you to the reviewers. I approached this test with genuine dedication, and I simply hope it can be evaluated with fairness.


Cheers!

Eugenio
