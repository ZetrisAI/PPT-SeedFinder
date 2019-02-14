#include "double.h"

const std::vector<int> double_s_sol[8] = {
	{7},
	{4},
	{},
	{},
	{0},
	{3},
	{1, 5, 6},
	{2}
};

const std::vector<int> double_z_sol[8] = {
	{4},
	{7},
	{},
	{},
	{1},
	{2},
	{0, 5, 6},
	{3}
};

const std::vector<int> double_s_next = {
	30, // pc3_s,
	31  // pc3_s_i
};

const std::vector<int> double_z_next = {
	32, // pc3_z,
	33  // pc3_z_i
};

void double_s(int* bag, int hold, solution* next) {
	if (hold != 0) return;

	int out;
	if (traverse_preserved_with_preserve(bag, hold, 6, &out, double_s_sol)) {
		for (int i = 0; i < double_s_next.size(); i++) {
			next->push_back(std::make_tuple(out, double_s_next[i]));
		}
	}
}

void double_z(int* bag, int hold, solution* next) {
	if (hold != 1) return;

	int out;
	if (traverse_preserved_with_preserve(bag, hold, 6, &out, double_z_sol)) {
		for (int i = 0; i < double_z_next.size(); i++) {
			next->push_back(std::make_tuple(out, double_z_next[i]));
		}
	}
}
