#include "pc3_i.h"

const std::vector<int> pc3_s_i_sol[8] = {
	{3},
	{3},
	{},
	{7},
	{},
	{7},
	{0, 1, 5, 6},
	{2, 4}
};

const std::vector<int> pc3_z_i_sol[8] = {
	{2},
	{2},
	{7},
	{},
	{},
	{7},
	{0, 1, 5, 6},
	{3, 4}
};

const std::vector<int> pc3_s_i_next = {
	35 // pc4_l_i
};

const std::vector<int> pc3_z_i_next = {
	37 // pc4_j_i
};

void pc3_s_i(int* bag, int hold, solution* next) {
	if (hold != 6) return;

	int out;
	if (traverse_preserved_with_preserve(bag, hold, 6, &out, pc3_s_i_sol)) {
		for (int i = 0; i < pc3_s_i_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc3_s_i_next[i]));
		}
	}
}

void pc3_z_i(int* bag, int hold, solution* next) {
	if (hold != 6) return;

	int out;
	if (traverse_preserved_with_preserve(bag, hold, 6, &out, pc3_z_i_sol)) {
		for (int i = 0; i < pc3_z_i_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc3_z_i_next[i]));
		}
	}
}
