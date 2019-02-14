#include "pc6.h"

const std::vector<int> pc6_sol[7] = {
	{},
	{6},
	{},
	{6},
	{0},
	{6},
	{2, 4}
};

const std::vector<int> pc6_m_sol[7] = {
	{6},
	{},
	{6},
	{},
	{1},
	{6},
	{3, 4}
};

void pc6(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc6_sol)) {
		for (int i = 0; i < pc6_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc6_next[i]));
		}
	}
}

void pc6_m(int* bag, int hold, solution* next) {
	int out;
	if (traverse(bag, hold, &out, pc6_m_sol)) {
		for (int i = 0; i < pc6_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc6_next[i]));
		}
	}
}
