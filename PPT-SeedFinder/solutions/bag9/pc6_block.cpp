#include "pc6_block.h"

const std::vector<int> pc6_block_sol[8] = {
	{},
	{6},
	{6},
	{},
	{0},
	{6},
	{4, 7},
	{3}
};

const std::vector<int> pc6_block_m_sol[8] = {
	{6},
	{},
	{},
	{6},
	{1},
	{6},
	{4, 7},
	{2}
};

void pc6_block(int* bag, int hold, solution* next) {
	if (hold != 5) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc6_block_sol)) {
		for (int i = 0; i < pc6_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc6_next[i]));
		}
	}
}

void pc6_block_m(int* bag, int hold, solution* next) {
	if (hold != 5) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc6_block_m_sol)) {
		for (int i = 0; i < pc6_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc6_next[i]));
		}
	}
}
