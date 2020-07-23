#pragma once
#include <vector>
#include "globalStructures.h"
class ConsoleController {
  Settings m_set;
  std::vector<std::string> decompose_command(std::string input);
  Command analyse_command(std::vector<std::string> command);
public:
  Command get_input();
};