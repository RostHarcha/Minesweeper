#include <vector>
#include <iostream>
#include "ConsoleDrawer.h"
#include "globalStructures.h"
#include <Windows.h>

int ConsoleDrawer::cell(int x, int y) {
  return y * set.size_x + x;
}

void ConsoleDrawer::draw_cell(CellState state, int sign) {
  auto set_color = [](const int bg, const int text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((bg << 4) | text));
  };
  auto print_cell = [set_color](const char icon, const int bg, const int text) {
    set_color(15, 0);
    std::cout << "[";
    set_color(bg, text);
    std::cout << icon;
    set_color(15, 0);
    std::cout << "]";
  };
  auto print_digit = [print_cell](const int digit) {
    print_cell(static_cast<char>(digit + static_cast<int>('0')), 15, digit);
  };
  switch (state) {
  case CellState::Closed:
    print_cell(' ', 15, 0);
    break;
  case CellState::Oppened:
    switch (sign) {
    case 0:
      print_cell('-', 15, 7);
      break;
    case 9:
      print_cell('@', 15, 4);
      break;
    default:
      print_digit(sign);
      break;
    }
    break;
  case CellState::Flag:
    print_cell('F', 10, 2);
    break;
  case CellState::Triggered:
    print_cell('@', 12, 0);
    break;
  }
}

void ConsoleDrawer::set_color(int bg, int text) {
}

void ConsoleDrawer::create(Settings _set, std::vector<int> _cell_sign) {
  set = _set;
  cell_sign = _cell_sign;
}

void ConsoleDrawer::draw(std::vector<CellState> state, std::string comment) {
  system("cls");
  set_color(15, 0);
  std::cout << comment << "\n  y \\ x\t ";
  for (int i = 0; i < set.size_x; i++) {
    if (i < 10) {
      std::cout << i << "   ";
    }
    if (9 < i and i < 100) {
      std::cout << i << "  ";
    }
  }
  std::cout << std::endl;
  for (int y = 0; y < set.size_y; y++) {
    std::cout << y << '\t';
    for (int x = 0; x < set.size_x; x++) {
      draw_cell(state[cell(x, y)], cell_sign[cell(x, y)]);
      std::cout << " ";
    }
    std::cout << std::endl;
  }
}

void ConsoleDrawer::non_signs_draw(Settings set, std::string comment) {
  std::vector<CellState> state;
  for (auto i = 0; i < set.size_x * set.size_y; i++) {
    state.push_back(CellState::Closed);
    cell_sign.push_back(0);
  }
  draw(state, comment);
}