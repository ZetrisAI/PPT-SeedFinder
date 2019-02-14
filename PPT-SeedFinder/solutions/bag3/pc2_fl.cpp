#include "pc2_fl.h"

const std::vector<int> pc2_fl_1_sol[7] = {
	{0, 3},
	{},
	{1},
	{2, 4, 5, 6},
	{},
	{},
	{}
};

const std::vector<int> pc2_fl_2_sol[7] = {
	{0, 2},
	{},
	{4, 5, 6},
	{},
	{},
	{},
	{1, 3}
};

void pc2_fl_1(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 0, &out, pc2_fl_1_sol)) {
		for (int i = 0; i < s_next.size(); i++) {
			next->push_back(std::make_tuple(out, s_next[i]));
		}
	}
}

void pc2_fl_2(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 0, &out, pc2_fl_2_sol)) {
		for (int i = 0; i < s_next.size(); i++) {
			next->push_back(std::make_tuple(out, s_next[i]));
		}
	}
}
