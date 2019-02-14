#include "pc5.h"

const std::vector<int> pc5_sol[7] = {
	{4},
	{2},
	{6},
	{6},
	{1},
	{3},
	{}
};

const std::vector<int> pc5_m_sol[7] = {
	{3},
	{4},
	{6},
	{6},
	{0},
	{2},
	{}
};

const std::vector<int> pc5_next = {
	44 // pc6
};

const std::vector<int> pc5_m_next = {
	45 // pc6_m
};

void pc5(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc5_sol)) {
		for (int i = 0; i < pc5_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc5_next[i]));
		}
	}
}

void pc5_m(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc5_m_sol)) {
		for (int i = 0; i < pc5_m_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc5_m_next[i]));
		}
	}
}
