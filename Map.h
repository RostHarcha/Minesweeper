#pragma once
#include "globalStructures.h"
#include <vector>

class Map {
  Settings m_set;
  std::vector<bool> mine;
  std::vector<CellState> state;
  std::vector<int> m_cell_sign;
  GameState game_state;
  Statistic stat;

  int cell(int x, int y);
  int get_cell_sign(const int x, const int y);
  bool get_mine(const int x, const int y);
  void create_vectors();
  void create_mines(const int first_x, const int first_y);
  void clear_mines();
  void clear_map();
  bool on_map(int x, int y);
  int mines_around(int x, int y);
  void flag(const int x, const int y);
  void remove_flag(const int x, const int y);
  void open(int x, int y);
  void open_map();
  void trigger_mine(int x, int y);
  CellState get_state(const int x, const int y);
  void set_state(const int x, const int y, CellState _state);
  void open_around(const int x, const int y);
  std::vector<int> create_cell_signs();
  void open_empty_cells(int cell_sign);

public:
  Map(Settings set, const Command first_command);
  Statistic get_stat(); 
  GameState get_game_state();
  void process_tick(Action action, int x, int y);
  std::vector<CellState> get_current_state();
  std::vector<int> get_cell_sign();
};