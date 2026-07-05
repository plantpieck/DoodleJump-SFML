/*
 * ========== Naming Convention Guideline ==========
 * Class names    : PascalCase
 * Function names : camelCase
 * Variable names : camelCase (Member variables prefix 'm')
 * Constant names : UPPER_SNAKE_CASE
 * =================================================
 */
#include "../include/Game.hpp"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "\n❌ FATAL ERROR: " << e.what() << "\n\n";
        return -1;
    }
    
    return 0;
}