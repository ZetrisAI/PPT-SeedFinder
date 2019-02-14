#include "pc2_sj.h"

const std::vector<int> pc2_sj_s_sol[7] = {
	{0, 2},
	{},
	{3, 4, 5, 6},
	{1},
	{},
	{},
	{}
};

const std::vector<int> pc2_sj_z_sol[7] = {
	{},
	{1, 3},
	{},
	{4, 5, 6},
	{0},
	{},
	{2}
};

void pc2_sj_s(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 0, &out, pc2_sj_s_sol)) {
		for (int i = 0; i < s_next.size(); i++) {
			next->push_back(std::make_tuple(out, s_next[i]));
		}
	}
}

void pc2_sj_z(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 1, &out, pc2_sj_z_sol)) {
		for (int i = 0; i < z_next.size(); i++) {
			next->push_back(std::make_tuple(out, z_next[i]));
		}
	}
}
