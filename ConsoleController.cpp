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
    output.x = std::atoi(command[1].c_str());
    output.y = std::atoi(command[2].c_str());
    break;
  }
  return output;
}

void ConsoleController::create(Settings _set) {
  set = _set;
}

Command ConsoleController::get_input() {
  std::string input;
  std::getline(std::cin, input);

  return analyse_command(decompose_command(input));
}