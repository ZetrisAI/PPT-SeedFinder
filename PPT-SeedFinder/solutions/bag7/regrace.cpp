#include "regrace.h"

const std::vector<int> regrace_sol[7] = {
	{2},
	{3},
	{},
	{},
	{},
	{2, 3},
	{0, 1, 5}
};

const std::vector<int> regrace_i_sol[7] = {
	{6},
	{6},
	{0, 5},
	{1, 5},
	{},
	{6},
	{}
};

const std::vector<int> regrace_next = {
	40, // pc5,
	41, // pc5_m,
	42, // pc5_o,
	43  // pc5_o_m
};

void regrace(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, regrace_sol)) {
		for (int i = 0; i < regrace_next.size(); i++) {
			next->push_back(std::make_tuple(out, regrace_next[i]));
		}
	}
}

void regrace_i(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, regrace_i_sol)) {
		for (int i = 0; i < regrace_next.size(); i++) {
			next->push_back(std::make_tuple(out, regrace_next[i]));
		}
	}
}
