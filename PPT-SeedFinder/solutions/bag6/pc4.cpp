#include "pc4.h"

const std::vector<int> pc4_l_sol[7] = {
	{},
	{},
	{6},
	{0, 1},
	{2},
	{},
	{3, 5}
};

const std::vector<int> pc4_j_sol[7] = {
	{},
	{},
	{0, 1},
	{6},
	{3},
	{},
	{2, 5}
};

void pc4_l(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc4_l_sol)) {
		for (int i = 0; i < pc4_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc4_next[i]));
		}
	}
}

void pc4_j(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc4_j_sol)) {
		for (int i = 0; i < pc4_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc4_next[i]));
		}
	}
}
