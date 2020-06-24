#include <iostream>
#include <Windows.h>
#include <ctime>
#include "globalStructures.h"
#include "ConsoleDrawer.h"
#include "Map.h"
#include "ConsoleController.h"
#include "AI.h"

constexpr auto DEBUG_MODE = 0;
constexpr auto AI_MODE = 0;

class Game {
private:
  Map map;
  ConsoleDrawer drawer;
  ConsoleController controller;
  AI ai;
  Settings set;
  std::string comment;
  GameState game_state = GameState::game;

  void update_comment(bool help) {
    if (help) {
      comment =
        "General view of the command: [action] <x> <y>\n"
        "|help\t\tcall for help\n"
        "|stop\t\tstop the game\n"
        "|open <x> <y>\topen the cell\n"
        "|+open <x> <y>\topen the cell and 8 cells around it\n"
        "|flag <x> <y>\tflag the cell\n"
        "|-flag <x> <y>\tremove flag from the cell\n";
      comment += "cells oppened: " + std::to_string(map.cells_oppened);
    }
    else {
      comment = "cells oppened: " + std::to_string(map.cells_oppened);
    }
    comment += "\\" + std::to_string(set.size_x * set.size_y);
    comment += "\t\tmines: " + std::to_string(set.mines);
    if (DEBUG_MODE) {
      comment += "\nDEBUG MODE";
    }
  }

  void tick() {
    Command input;
    if (!AI_MODE) {
      input = controller.get_input();
    }
    else {
      //input = ai.get_input();
    }

    if (input.action == Action::Stop) {
      game_state = GameState::loss;
      return;
    }

    map.process_tick(input.action, input.x, input.y);
    game_state = map.game_state;

    if (input.action == Action::Help) {
      update_comment(1);
    }
    else {
      update_comment(0);
    }

    drawer.draw(map.get_current_state(), comment);
    return;
  }

public:
  Game(Settings _set) {
    system("color F0");
    switch (_set.mines) {
    case 1:
      _set.mines = 5;
      break;
    default:
      _set.mines = 10;
      break;
    case 3:
      _set.mines = 30;
      break;
    }
    _set.mines = (_set.size_x * _set.size_y) * _set.mines / 100;
    set = _set;

    srand(set.seed);

    controller.create(set);

    Command first_command;
    do {
      drawer.non_signs_draw(set, "type 'open <x> <y>' to open the cell");
      first_command = controller.get_input();
    } while (first_command.x < 0 && first_command.y < 0 && first_command.action != Action::Open);

    map.create(set, first_command.x, first_command.y);
    drawer.create(set, map.get_cell_signs());

    update_comment(0);
    drawer.draw(map.get_current_state(), comment);
    for (;;) {
      switch (game_state) {
      case GameState::game:
        tick();
        break;
      case GameState::win:
        drawer.draw(map.get_current_state(), comment);
        std::cout << "\n###Congrats! You won this game!###\n";
        return;
      case GameState::loss:
        drawer.draw(map.get_current_state(), comment);
        std::cout << "\nGame over!\n";
        return;
      }
    }
  }
};

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