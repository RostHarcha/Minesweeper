#include <iostream>
#include <ctime>

#include "globalStructures.h"
#include "Game.h"

int main() {
  Settings set;
  if (!DEBUG_MODE) {
    std::cout << "Width: ";
    std::cin >> set.size_x;
    std::cout << "Height: ";
    std::cin >> set.size_y;
    std::cout << "Difficulty(1 - 3): ";
    std::cin >> set.mines;
    set.seed = time(0);
  }

  Game game(set);
  return 0;
}