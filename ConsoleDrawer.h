#pragma once
#include "globalStructures.h"
#include <vector>
class ConsoleDrawer {
  Settings set;
  std::vector<int> cell_sign;
  int cell(int x, int y);
  void draw_cell(CellState state, int sign);
  void set_color(int bg, int text);
public:
  void create(Settings _set, std::vector<int> _cell_sign);
  void draw(std::vector<CellState> state, std::string comment);
  void non_signs_draw(Settings _set, std::string comment);
};