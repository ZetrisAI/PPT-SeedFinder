#include "pc3.h"

const std::vector<int> pc3_s_sol[8] = {
	{3},
	{3},
	{},
	{7},
	{},
	{7},
	{7},
	{2, 4}
};

const std::vector<int> pc3_z_sol[8] = {
	{2},
	{2},
	{7},
	{},
	{},
	{7},
	{7},
	{3, 4}
};

const std::vector<int> pc3_s_next = {
	34 // pc4_l
};

const std::vector<int> pc3_z_next = {
	36 // pc4_j
};

void pc3_s(int* bag, int hold, solution* next) {
	if (hold != 6) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc3_s_sol)) {
		for (int i = 0; i < pc3_s_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc3_s_next[i]));
		}
	}
}

void pc3_z(int* bag, int hold, solution* next) {
	if (hold != 6) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc3_z_sol)) {
		for (int i = 0; i < pc3_z_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc3_z_next[i]));
		}
	}
}
