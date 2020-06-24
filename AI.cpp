#include <vector>
#include "AI.h"

int AI::cell(int x, int y) {
	return y * set.size_x + x;
}

Command AI::get_input(std::vector<CellState> _state) {
	state = _state;
	Command output = rand_coords();
	//output.action = Action::Open;
	output.action = rand_action();
	return output;
}

Command AI::create(Settings _set) {
	set = _set;
	return rand_coords();
}

Command AI::rand_coords() {
	int x = rand() % set.size_x;
	int y = rand() % set.size_y;
	return { Action::NONE, x, y };
}

Action AI::rand_action() {
	int rnd = rand() % 2;
	if (rnd == 0) {
		return Action::Open;
	}
	else {
		return Action::Flag;
	}
}

Command AI::find_corner_mine(const int _x, const int _y) {
	std::vector<CellState> sector;
	int mines = 0;
	for (int y = _y - 1; y <= _y + 1; y++) {
		for (int x = _x - 1; x <= _x + 1; x++) {
			if (on_map(x, y)) {
				sector.push_back(state[cell(x, y)]);
				mines++;
			}
		}
	}
	if (mines != sector.size() - 1) {
		return { Action::NONE };
	}
	//for(int i = 0; i < )
}

bool AI::on_map(int x, int y) {
	if (x < set.size_x and x >= 0 and y < set.size_y and y >= 0) return 1;
	else return 0;
}