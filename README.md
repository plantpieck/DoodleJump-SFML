# 🐰 Doodle Jump Clone - Phase 1

This project is a C++ implementation of the classic endless-jumper game, **Doodle Jump**, built using the **SFML 3** library. It was developed as Phase 1 of an academic project, focusing heavily on Clean Code, Object-Oriented Programming (OOP), and strict manual memory management.

## ✨ Features & Technical Highlights

* **Classic Gameplay Mechanics:**
  * Endless upward scrolling using `sf::View` for optimized camera movement (the camera follows the player upward, preventing the physical shifting of all game objects).
  * 4 distinct platform types: 
    * **Normal (Green):** Standard bounce.
    * **Moving (Blue):** Moves horizontally and bounces off screen edges.
    * **Breakable (Brown):** Breaks and falls when stepped on (no jump triggered).
    * **Spring-loaded:** Triggers a Super Jump and includes a dynamic compression animation using `setTextureRect`.
  * Dynamic platform generation algorithm ensuring no dead-ends.
  * Score tracking based on height and persistent High Score saving via `highscore.txt`.
  * Screen wrap-around (going off the left edge makes the player appear on the right edge, and vice versa).

* **Strict Academic Memory Management:**
  * Comprehensive use of raw pointers and explicit `delete` operations in destructors.
  * Implementation of `virtual destructors` in base classes (e.g., `Platform`) to ensure zero memory leaks during polymorphism.

* **Clean Code & Architecture:**
  * Complete encapsulation within the `Game` class.
  * Zero global variables.
  * Safe type casting using `dynamic_cast` for handling specific platform collisions.
  * Generic Template-based `ResourceManager` class for efficient and scalable asset loading.
  * Robust error handling using `try-catch` blocks in the main execution flow to prevent silent crashes.
  * Strict adherence to predefined naming conventions (PascalCase for classes, camelCase for functions/variables, etc.).

## 🛠️ Prerequisites

To compile and run this game, you will need:
* A modern C++ Compiler (supporting C++17 or higher)
* **SFML 3** (Simple and Fast Multimedia Library) installed on your system.

*Note for macOS users: You can install SFML 3 via Homebrew using `brew install sfml`.*

## 🚀 How to Build and Run

1. **Clone the repository:**
   ```bash
   git clone <your-repository-url>
   cd <your-repository-folder>
   ```

2. **Compile the game:**
   Using the provided Makefile:
   ```bash
   make
   ```

3. **Run the executable:**
   ```bash
   ./doodle_jump
   ```
   *(Note: Adjust the executable name if your Makefile outputs a different name).*

## 🎮 Controls

* **Left Arrow:** Move Doodle left
* **Right Arrow:** Move Doodle right
* **Mouse (Left Click):** Navigate through Menus (Start Game, Restart, Main Menu)

## 📂 Project Structure

```text
├── assets/          # Images and textures (sprites, backgrounds, UI buttons)
├── fonts/           # TrueType fonts used for UI text
├── include/         # .hpp header files (Class declarations)
├── src/             # .cpp source files (Class implementations & main.cpp)
├── Makefile         # Build configuration
└── README.md        # Project documentation
```

## 🚧 Future Roadmap (Phase 2)
This repository is actively maintained. Phase 2 of the project will be implemented in this same repository, introducing new mechanics, enemies, power-ups, and advanced gameplay features. Stay tuned!

## 👨‍💻 Author
* **Hedyeh Nayebi**
* Engineering Science Student, University of Tehran.
* Developed for the Advanced Programming Course - Phase 1.