#include "pc1.h"

const std::vector<int> pc1_j_slope_l_sol[7] = {
	{3},
	{3},
	{4},
	{6},
	{},
	{6},
	{2}
};

const std::vector<int> pc1_j_flat_l_sol[7] = {
	{1},
	{3},
	{4},
	{6},
	{},
	{6},
	{2}
};

const std::vector<int> pc1_l_slope_j_sol[7] = {
	{2},
	{2},
	{6},
	{4},
	{},
	{6},
	{3}
};

const std::vector<int> pc1_l_flat_j_sol[7] = {
	{2},
	{0},
	{6},
	{4},
	{},
	{6},
	{3}
};

void pc1_j_slope_l(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc1_j_slope_l_sol)) {
		for (int i = 0; i < slope_l_next.size(); i++) {
			next->push_back(std::make_tuple(out, slope_l_next[i]));
		}
	}
}

void pc1_j_flat_l(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc1_j_flat_l_sol)) {
		for (int i = 0; i < flat_l_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_l_next[i]));
		}
	}
}

void pc1_l_slope_j(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc1_l_slope_j_sol)) {
		for (int i = 0; i < slope_j_next.size(); i++) {
			next->push_back(std::make_tuple(out, slope_j_next[i]));
		}
	}
}

void pc1_l_flat_j(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc1_l_flat_j_sol)) {
		for (int i = 0; i < flat_j_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_j_next[i]));
		}
	}
}
