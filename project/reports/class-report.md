# Class diagram verification report

**🎨 [Open the colour-coded comparison in draw.io](../diagrams/output/class-comparison.drawio)** — page 1 is the design, page 2 the implementation. Every difference below is coloured there; hover over a coloured element to see what differs.

| | File | Made by |
|---|---|---|
| **Design** | [`diagrams/input/class.drawio`](../diagrams/input/class.drawio) → [`diagrams/output/class-design.mmd`](../diagrams/output/class-design.mmd) | a person, in draw.io |
| **Implemented design** | [`diagrams/output/class-implemented.mmd`](../diagrams/output/class-implemented.mmd), from [`impl/include/`](../impl/include) | extracted from the C++ headers by libclang |

> In VS Code, click a link to open the file. `.drawio` files open in the *Draw.io Integration* extension, which VS Code offers to install for this repository.

## Overview

**Alignment: 53.2 %** — 41 of the 77 elements that appear in either diagram are identical in both.

| | Classes | Attributes | Methods | Relations | **Total** |
|---|--:|--:|--:|--:|--:|
| In design | 9 | 29 | 10 | 4 | **52** |
| In implemented design | 10 | 34 | 19 | 8 | **71** |
| ✅ Identical | 9 | 27 | 5 | 0 | **41** |
| ✏️ Changed | 0 | 0 | 5 | 0 | **5** |
| ➖ Missing | 0 | 2 | 0 | 4 | **6** |
| ➕ Extra | 1 | 7 | 9 | 8 | **25** |
| **Alignment** | 90.0 % | 75.0 % | 26.3 % | 0.0 % | **53.2 %** |

### What the numbers mean

| | Meaning |
|---|---|
| **In design** | Elements drawn in the design. |
| **In implemented design** | Elements found in the implementation. |
| **✅ Identical** | In both, and every detail matches: types, parameters, visibility, relation kind, cardinality. |
| **✏️ Changed** | In both, but at least one detail differs. |
| **➖ Missing** | In the design, absent from the implementation. The AI left it out. |
| **➕ Extra** | In the implementation, absent from the design. The AI added it. |
| **Alignment** | Identical ÷ (Identical + Changed + Missing + Extra). 100 % means the implementation is exactly the design. |

An *element* is one class, one attribute (or enum value), one method, or one relation. Every element is counted exactly once, so the table adds up: Identical + Changed + Missing = In design, and Identical + Changed + Extra = In implemented design. Every Changed, Missing and Extra element is listed below.

## Differences

### ✏️ Changed (5)

| Element | What differs | Design | Implemented |
|---|---|---|---|
| method `Game.add_review()` | return type | void | Review |
| method `Repository.add_feature_request()` | return type | void | Request |
| method `Repository.show_requests()` | return type | void | vector<Request> |
| method `Review.update_score()` | parameters | () | (int) |
| method `User.create_new_game()` | return type | void | Game |

### ➖ Missing (6)

| Element | As drawn in the design |
|---|---|
| attribute `Game.creator` | `-User creator` |
| attribute `Game.game_repository` | `-Repository game_repository` |
| relation `Game → Repository` | `Game *-- Repository` |
| relation `Game → Review` | `Game o-- Review` |
| relation `Game → User (1)` | `Game o-- User : 1` |
| relation `Repository → Request` | `Repository o-- Request` |

### ➕ Extra (25)

| Element | As found in the implementation |
|---|---|
| class `Comment` | `class Comment` |
| attribute `Comment.description` | `-string description` |
| attribute `Comment.id` | `-uuid id` |
| attribute `Comment.title` | `-string title` |
| attribute `Game.categories` | `-vector<Category> categories` |
| attribute `Game.state` | `-State state` |
| attribute `Request.state` | `-State state` |
| attribute `User.state` | `-State state` |
| method `Comment.update()` | `+update(string, string) void` |
| method `Game.handle()` | `+handle(Event) void` |
| method `Game.invalid_repo()` | `-invalid_repo() bool` |
| method `Game.valid_repo()` | `-valid_repo() bool` |
| method `Repository.validate()` | `+validate() bool` |
| method `Request.handle()` | `+handle(Event) void` |
| method `Request.update()` | `+update(string, string) void` |
| method `User.handle()` | `+handle(Event) void` |
| method `User.no_active_suspension()` | `-no_active_suspension() bool` |
| relation `Comment → User (user)` | `Comment o-- User : user` |
| relation `Game → Repository (game_repository)` | `Game *-- Repository : game_repository` |
| relation `Game → Review (reviews)` | `Game o-- Review : reviews` |
| relation `Game → User (creator)` | `Game o-- User : creator` |
| relation `Repository → Request (feature_requests)` | `Repository o-- Request : feature_requests` |
| relation `Request → Comment` | `Request <|-- Comment` |
| relation `Review → Comment` | `Review <|-- Comment` |
| relation `User → Game (game_library)` | `User --> Game : game_library` |

### Not counted

Dependencies the design already implies through a method signature. Designs rarely draw these arrows, so finding one in the implementation is not a difference.

- relation `Repository ..> User`

### ⚠️ Warnings (5)

Things that could not be read or counted, in the design or in the implementation. They are not in any count above, so check them by hand.

- shape 'Comment{abstract}' looks like a class but its name is not readable; skipped
- Category: could not read member '...'; skipped
- a connector labelled '' is not attached to two classes; skipped
- a connector labelled '' is not attached to two classes; skipped
- a connector labelled '' is not attached to two classes; skipped

---

_Generated by `tools/verify.py`. Deterministic: the same two diagrams always produce this exact report._
