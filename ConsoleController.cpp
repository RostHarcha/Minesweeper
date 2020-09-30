#include "ConsoleController.h"
#include <sstream>
#include <iostream>
#include <string>

std::vector<std::string> ConsoleController::decompose_command(std::string input) {
  std::vector<std::string> output;
  std::stringstream ss(input);
  std::string buffer;
  while (ss >> buffer) {
    output.push_back(buffer);
  }
  return output;
}

Command ConsoleController::analyse_command(std::vector<std::string> command) {
  Command output;
  switch (command.size()) {
  case 1:
    if (command[0] == "stop") {
      output.action = Action::Stop;
    }
    if (command[0] == "help") {
      output.action = Action::Help;
    }
    break;
  case 3:
    if (command[0] == "open") {
      output.action = Action::Open;
    }
    if (command[0] == "flag") {
      output.action = Action::Flag;
    }
    if (command[0] == "-flag") {
      output.action = Action::RemoveFlag;
    }
    if (command[0] == "+open") {
      output.action = Action::OpenAround;
    }
    int x = std::atoi(command[1].c_str()), y = std::atoi(command[2].c_str());
    if (on_map(x, y)) {
      output.x = x, output.y = y;
    }
    break;
  }
  return output;
}

bool ConsoleController::on_map(const int x, const int y) {
  if (x < m_set.size_x && x >= 0 && y < m_set.size_y && y >= 0) return 1;
  return 0;
}

Command ConsoleController::get_input() {
  std::string input;
  std::getline(std::cin, input);

  return analyse_command(decompose_command(input));
}

ConsoleController::ConsoleController(Settings set) {
  m_set = set;
}