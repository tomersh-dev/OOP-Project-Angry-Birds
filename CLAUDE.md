# Project Overview
This is a 2D physics-based game built in C++ using **SFML 3** and **Box2D**. The architecture strictly follows OOP principles, SOLID guidelines, and modern C++ practices.

## Architectural Rules & Constraints (CRITICAL)
1. **NO Magic Numbers:** All constants (speeds, physics properties, scores, UI sizes) MUST be defined in `Constants.h` inside appropriately named namespaces (e.g., `Physics`, `Scores`, `Materials`).
2. **SFML 3 Strictness:** 
   - `sf::Sprite` NO LONGER has a default constructor. It must be initialized with a texture in the initializer list.
   - `sf::FloatRect` uses `{position, size}` structure instead of `left, top, width, height`.
   - Use `sf::PrimitiveType::Lines` (not LineStrip) for rendering the Slingshot rubber band.
3. **No RTTI for Core Logic:** Avoid `dynamic_cast`. Rely on polymorphism, virtual functions, and decoupled design (e.g., separate vectors for different entity types in the Controller).
4. **Box2D Memory Management:** Always call `b2World::DestroyBody()` (via our `DestroyPhysics` method) before deleting the C++ object.
5. **CMake Structure:** Resources are loaded dynamically. `assets` and `levels` folders are copied to the build directory via CMake `file(COPY...)`. Use relative paths like `levels/level1.txt`.

## Current State of the Codebase
- `GameObject` base class encapsulating `sf::Sprite` and `b2Body*`.
- `Controller` acts as the facade/game loop manager.
- `ObjectFactory` creates objects based on a parsed text file from `Board`.
- `Camera` system handles smooth panning (right-click) and entity following, bounded by `sf::FloatRect`.
- `Slingshot` handles click-and-drag physics impulse mechanics.

---

## Roadmap & Next Objectives

### 1. Refactor: Projectile Hierarchy & Magazine System
- Rename the existing `Bird` class to `Projectile`.
- Make `Projectile` an **Abstract Base Class**.
- Create 2 inherited classes: `NormalProjectile` and `BoostProjectile`.
- **The Ability System:** Add `virtual void ActivateAbility() = 0;`. Abilities trigger on left-click while the projectile is mid-air.
  - `NormalProjectile`: Does nothing.
  - `BoostProjectile`: Applies a strong horizontal linear impulse to itself.
- **Magazine System (Queue):**
  - The level text file will dictate the loadout via a keyword (e.g., `MAGAZINE NORMAL NORMAL BOOST NORMAL`).
  - The `Controller` will parse this into a queue of `sf::Sprite` elements shown on the bottom-left HUD.
  - A physical Box2D `Projectile` object is ONLY instantiated by the `ObjectFactory` when its turn arrives and it is loaded into the `Slingshot`. The HUD magazine does not contain physical bodies.

### 2. Refactor: Block Hierarchy
- Make `Block` an **Abstract Base Class**.
- Create inherited classes: `WoodBlock`, `IceBlock`, `StoneBlock`.
- Map them to unique values in `Constants.h`:
  - `Density`, `Friction`, `Restitution`.
  - `Health` (ice breaks easily, stone is tough).
  - `ScoreValue` (e.g., breaking Stone gives more points).

### 3. Design Patterns
- Refactor `AssetManager` into a **Singleton**.
- Create an `AudioManager` **Singleton** to handle sounds (Slingshot pull, Projectile flying in the air, Block breaking).

### 4. Game Loop & Logic Implementation
- **Contact Listener:** Implement collision calculations using Box2D impulses. Apply `TakeDamage()` to Blocks and Targets based on the collision impact force.
- **Rest Detection (Next Turn):** The current turn ends when the fired Projectile is destroyed/out of bounds, OR when all bodies on screen settle (velocities drop near zero for a specific duration). Once settled, the camera returns to the Slingshot and the next Projectile is loaded from the Magazine.
- **Game States:** Implement a State Machine in the `Controller` (`MainMenu`, `Playing`, `LevelTransition`, `GameOver`).
- **Scoring & HUD:** Display the current score, driven by the destruction of Targets and Blocks. Add a level-clear bonus for remaining Projectiles in the Magazine. Auto-transition to the next level file upon destroying all Targets.

Whenever I ask you to implement a feature, refer to this document first, adhere strictly to the OOP constraints, and implement the features step-by-step.