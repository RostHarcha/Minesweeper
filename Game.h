#pragma once
#include "globalStructures.h"
#include "ConsoleController.h"
#include "ConsoleDrawer.h"
#include "Map.h"
#include "AI.h"

class Game {
private:
  Settings m_set;
  ConsoleController controller;
  Map map;
  ConsoleDrawer drawer;
  AI ai;
  std::string m_comment;

  void update_comment(bool help);
  GameState tick();
  Settings preset(Settings set);
  Command first_command(Settings set);
  void start(GameState game_state);
  
public:
  Game(Settings set);
};