#include "pc4_i.h"

const std::vector<int> pc4_l_i_sol[8] = {
	{},
	{},
	{1},
	{6},
	{3},
	{},
	{7},
	{0, 2, 5}
};

const std::vector<int> pc4_j_i_sol[8] = {
	{},
	{},
	{6},
	{0},
	{2},
	{},
	{7},
	{1, 3, 5}
};

void pc4_l_i(int* bag, int hold, solution* next) {
	if (hold != 6) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc4_l_i_sol)) {
		for (int i = 0; i < pc4_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc4_next[i]));
		}
	}
}

void pc4_j_i(int* bag, int hold, solution* next) {
	if (hold != 6) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc4_j_i_sol)) {
		for (int i = 0; i < pc4_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc4_next[i]));
		}
	}
}
