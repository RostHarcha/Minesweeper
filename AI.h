#pragma once
#include "globalStructures.h"

class AI {
private:
	Settings set;
	std::vector<CellState> state;
	Command rand_coords();
	Action rand_action();
	Command find_corner_mine(const int _x, const int _y);
	bool on_map(const int x, const int y);
	int cell(int x, int y);
public:
	Command get_input(std::vector<CellState> state);
	Command create(Settings _set);
};