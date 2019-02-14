#include "pc2_sl.h"

const std::vector<int> pc2_sl_s_sol[7] = {
	{0, 2},
	{},
	{4, 5, 6},
	{},
	{1},
	{},
	{3}
};

const std::vector<int> pc2_sl_z_sol[7] = {
	{},
	{1, 3},
	{0},
	{2, 4, 5, 6},
	{},
	{},
	{}
};

void pc2_sl_s(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 0, &out, pc2_sl_s_sol)) {
		for (int i = 0; i < s_next.size(); i++) {
			next->push_back(std::make_tuple(out, s_next[i]));
		}
	}
}

void pc2_sl_z(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 1, &out, pc2_sl_z_sol)) {
		for (int i = 0; i < z_next.size(); i++) {
			next->push_back(std::make_tuple(out, z_next[i]));
		}
	}
}
