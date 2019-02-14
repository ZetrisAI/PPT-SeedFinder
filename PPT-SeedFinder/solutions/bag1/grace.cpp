#include "grace.h"

const std::vector<int> grace_sol[7] = {
	{2},
	{3},
	{},
	{},
	{},
	{2, 3},
	{0, 1, 5}
};

const std::vector<int> grace_i_sol[7] = {
	{6},
	{6},
	{0, 5},
	{1, 5},
	{},
	{6},
	{}
};

const std::vector<int> grace_next = {
	8,  // pc1_j_slope_l,
	9,  // pc1_j_flat_l,
	10, // pc1_l_slope_j,
	11  // pc1_l_flat_j
};

void grace(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, grace_sol)) {
		for (int i = 0; i < grace_next.size(); i++) {
			next->push_back(std::make_tuple(out, grace_next[i]));
		}
	}
}

void grace_i(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, grace_i_sol)) {
		for (int i = 0; i < grace_next.size(); i++) {
			next->push_back(std::make_tuple(out, grace_next[i]));
		}
	}
}
