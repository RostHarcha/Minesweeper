#include "Game.h"
#include <iostream>

void Game::update_comment(bool help) {
  Statistic stat = map.get_stat();
  m_comment = "";
  if (help) {
    m_comment +=
      "General view of the command: [action] <x> <y>\n"
      "|help\t\tcall for help\n"
      "|stop\t\tstop the game\n"
      "|open <x> <y>\topen the cell\n"
      "|+open <x> <y>\topen the cell and 8 cells around it\n"
      "|flag <x> <y>\tflag the cell\n"
      "|-flag <x> <y>\tremove flag from the cell\n";
  }
  m_comment += "cells oppened: " + std::to_string(stat.cells_oppened);
  m_comment += "/" + std::to_string(m_set.size_x * m_set.size_y);
  m_comment += "\t\tflags/mines: " + std::to_string(stat.flags) + "/" + std::to_string(m_set.mines);
  if (DEBUG_MODE) {
    m_comment += "\nDEBUG MODE";
  }
}

GameState Game::tick() {
  Command input;
  if (!AI_MODE) {
    input = controller.get_input();
  }
  else {
    //input = ai.get_input();
  }

  if (input.action == Action::Stop) {
    return GameState::loss;
  }

  map.process_tick(input.action, input.x, input.y);

  if (input.action == Action::Help) {
    update_comment(1);
  }
  else {
    update_comment(0);
  }

  drawer.draw(map.get_current_state(), m_comment);
  return map.get_game_state();
}

Settings Game::preset(Settings set) {
  // setting percent of mines on the map
  switch (set.mines) {
  case 1:
    set.mines = 5;
    break;
  case 3:
    set.mines = 30;
    break;
  default:
    set.mines = 10;
    break;
  }

  // setting number of mines
  set.mines = (set.size_x * set.size_y) / 100 * set.mines;

  // setting seed
  srand(set.seed);
  
  return set;
}

Command Game::first_command(Settings set) {
  Command command;
  ConsoleDrawer localDrawer(set);
  do {
    localDrawer.non_signs_draw("type 'open <x> <y>' to open the cell");
    command = controller.get_input();
  } while (command.x < 0 && command.y < 0 && command.action != Action::Open);
  return command;
}

void Game::start() {
  GameState game_state = GameState::game;
  for (;;) {
    switch (game_state) {
    case GameState::game:
      game_state = tick();
      break;
    case GameState::win:
      drawer.draw(map.get_current_state(), m_comment);
      std::cout << "\n###Congrats! You won this game!###\n";
      return;
    case GameState::loss:
      drawer.draw(map.get_current_state(), m_comment);
      std::cout << "\nGame over!\n";
      return;
    }
  }
}

Game::Game(Settings set)
  :m_set(preset(set)),
  controller(),
  map(m_set, first_command(m_set)),
  drawer(m_set, map.get_cell_signs()),
  ai(),
  m_comment("type 'open <x> <y>' to open the cell")
{
  system("color F0");
  update_comment(0);
  drawer.draw(map.get_current_state(), m_comment);
  start();
}