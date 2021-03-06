#include <vector>
#include "Map.h"

int Map::cell(int x, int y) {
  return y * m_set.size_x + x;
}

int Map::get_cell_sign(const int x, const int y) {
  return m_cell_sign[cell(x, y)];
}

bool Map::get_mine(const int x, const int y) {
  return mine[cell(x, y)];
}

std::vector<CellState> Map::get_current_state() {
  return state;
}

std::vector<int> Map::get_cell_sign() {
  return m_cell_sign;
}

CellState Map::get_state(const int x, const int y) {
  return state[cell(x, y)];
}

Statistic Map::get_stat() {
  return stat;
}

GameState Map::get_game_state() {
  return game_state;
}

void Map::set_state(const int x, const int y, CellState _state) {
  if (on_map(x, y)) {
    state[cell(x, y)] = _state;
  }
}

void Map::create_vectors() {
  auto map_size = m_set.size_x * m_set.size_y;
  mine.resize(map_size);
  state.resize(map_size);
}

void Map::create_mines(const int first_x, const int first_y) {
  int mines_planted = 0;
  while (mines_planted <= m_set.mines) {
    int rand_cell = rand() % (m_set.size_x * m_set.size_y);
    if (rand_cell == cell(first_x, first_y)) {
      continue;
    }

    if (mine[rand_cell] == 0) {
      mine[rand_cell] = 1;
      mines_planted++;
    }
  }
}

void Map::clear_mines() {
  for (auto i = 0; i < mine.size(); i++) {
    mine[i] = false;
  }
}

void Map::clear_map() {
  for (int i = 0; i < state.size(); i++) {
    state[i] = CellState::Closed;
  }
}

bool Map::on_map(int x, int y) {
  if (x < m_set.size_x && x >= 0 && y < m_set.size_y && y >= 0) {
    return 1;
  }
  return 0;
}

int Map::mines_around(int _x, int _y) {
  int output = 0;
  if (get_mine(_x, _y)) {
    return 9;
  }

  for (int y = _y - 1; y <= _y + 1; y++) {
    for (int x = _x - 1; x <= _x + 1; x++) {
      if (on_map(x, y)) {
        if (get_mine(x, y)) {
          output++;
        }
      }
    }
  }
  return output;
}

std::vector<int> Map::create_cell_signs() {
  std::vector<int> output;
  for (int y = 0; y < m_set.size_y; y++) {
    for (int x = 0; x < m_set.size_x; x++) {
      output.push_back(mines_around(x, y));
    }
  }
  return output;
}

void Map::flag(const int x, const int y) {
  if (get_state(x, y) != CellState::Closed) {
    return;
  }
  set_state(x, y, CellState::Flag);
}

void Map::remove_flag(const int x, const int y) {
  if (get_state(x, y) != CellState::Flag) {
    return;
  }
  set_state(x, y, CellState::Closed);
}

void Map::open_map() {
  for (int i = 0; i < state.size(); i++) {
    state[i] = CellState::Oppened;
  }
  stat.cells_oppened = m_set.size_x * m_set.size_y;
}

void Map::trigger_mine(int x, int y) {
  game_state = GameState::loss;
  open_map();
  set_state(x, y, CellState::Triggered);
}

void Map::open_around(const int x0, const int y0) {
  for (int y = y0 - 1; y <= y0 + 1; y++) {
    for (int x = x0 - 1; x <= x0 + 1; x++) {
      if (on_map(x, y) && get_state(x, y) == CellState::Closed) {
        if (get_mine(x, y)) {
          trigger_mine(x, y);
          return;
        }
        set_state(x, y, CellState::Oppened);
        stat.cells_oppened++;
      }
    }
  }
}

void Map::open_empty_cells(int cell_sign) {
  if (cell_sign != 0) {
    return;
  }
  for (int i = 0; i < 10; i++) {
    for (int y = 0; y < m_set.size_y; y++) {
      for (int x = 0; x < m_set.size_x; x++) {
        if (get_state(x, y) == CellState::Oppened) {
          if (get_cell_sign(x, y) == 0) {
            open_around(x, y);
          }
        }
      }
    }
  }
}

void Map::open(int x, int y) {
  if (get_mine(x, y)) {
    trigger_mine(x, y);
    return;
  }
  if (get_state(x, y) != CellState::Closed) {
    return;
  }
  set_state(x, y, CellState::Oppened);
  stat.cells_oppened++;
  open_empty_cells(get_cell_sign(x, y));
}

Map::Map(Settings set, const Command first)
  :m_set(set),
  game_state(GameState::game),
  stat{ 0, 0 }
{
  create_vectors();
  clear_map();
  clear_mines();
  create_mines(first.x, first.y);
  set_state(first.x, first.y, CellState::Oppened);
  stat.cells_oppened++;
  m_cell_sign = create_cell_signs();
  open_empty_cells(get_cell_sign(first.x, first.y));
}

void Map::process_tick(Action action, int x, int y) {
  if (!on_map(x, y)) {
    return;
  }

  switch (action) {
  case Action::Open:
    open(x, y);
    break;
  case Action::Flag:
    flag(x, y);
    break;
  case Action::RemoveFlag:
    remove_flag(x, y);
    break;
  case Action::OpenAround:
    open_around(x, y);
    break;
  }

  if (stat.cells_oppened == m_set.size_x * m_set.size_y - (m_set.mines + 1)) {
    game_state = GameState::win;
  }
}