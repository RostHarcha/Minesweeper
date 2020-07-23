#pragma once
#include "globalStructures.h"
#include <vector>
class ConsoleDrawer {
  Settings m_set;
  std::vector<int> m_cell_sign;
  int cell(int x, int y);
  void draw_cell(CellState state, int sign);
  std::vector<int> empty_signs(Settings set);
public:
  ConsoleDrawer(Settings set, std::vector<int> cell_sign);
  ConsoleDrawer(Settings set);
  void draw(std::vector<CellState> state, std::string comment);
  void non_signs_draw(std::string comment);
};