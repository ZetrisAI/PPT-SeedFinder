#include "pc1_zs.h"

const std::vector<int> pc1_z_slope_l_sol[8] = {
	{3},
	{3},
	{7},
	{6},
	{},
	{6},
	{2},
	{4}
};

const std::vector<int> pc1_z_flat_l_sol[8] = {
	{1},
	{3},
	{7},
	{6},
	{},
	{6},
	{2},
	{4}
};

const std::vector<int> pc1_s_slope_j_sol[8] = {
	{2},
	{2},
	{6},
	{7},
	{},
	{6},
	{3},
	{4}
};

const std::vector<int> pc1_s_flat_j_sol[8] = {
	{2},
	{0},
	{6},
	{7},
	{},
	{6},
	{3},
	{4}
};

void pc1_z_slope_l(int* bag, int hold, solution* next) {
	if (hold != 1) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc1_z_slope_l_sol)) {
		for (int i = 0; i < slope_l_next.size(); i++) {
			next->push_back(std::make_tuple(out, slope_l_next[i]));
		}
	}
}

void pc1_z_flat_l(int* bag, int hold, solution* next) {
	if (hold != 1) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc1_z_flat_l_sol)) {
		for (int i = 0; i < flat_l_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_l_next[i]));
		}
	}
}

void pc1_s_slope_j(int* bag, int hold, solution* next) {
	if (hold != 0) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc1_s_slope_j_sol)) {
		for (int i = 0; i < slope_j_next.size(); i++) {
			next->push_back(std::make_tuple(out, slope_j_next[i]));
		}
	}
}

void pc1_s_flat_j(int* bag, int hold, solution* next) {
	if (hold != 0) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc1_s_flat_j_sol)) {
		for (int i = 0; i < flat_j_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_j_next[i]));
		}
	}
}
