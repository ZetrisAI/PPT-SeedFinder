#include "pc2_fj.h"

const std::vector<int> pc2_fj_1_sol[7] = {
	{},
	{1, 2},
	{3, 4, 5, 6},
	{0},
	{},
	{},
	{}
};

const std::vector<int> pc2_fj_2_sol[7] = {
	{},
	{1, 3},
	{},
	{4, 5, 6},
	{},
	{},
	{0, 2}
};

void pc2_fj_1(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 1, &out, pc2_fj_1_sol)) {
		for (int i = 0; i < z_next.size(); i++) {
			next->push_back(std::make_tuple(out, z_next[i]));
		}
	}
}

void pc2_fj_2(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 1, &out, pc2_fj_2_sol)) {
		for (int i = 0; i < z_next.size(); i++) {
			next->push_back(std::make_tuple(out, z_next[i]));
		}
	}
}
