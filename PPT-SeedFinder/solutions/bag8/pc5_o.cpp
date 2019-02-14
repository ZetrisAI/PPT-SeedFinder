#include "pc5_o.h"

const std::vector<int> pc5_o_sol[7] = {
	{4},
	{2},
	{6},
	{6},
	{1},
	{0, 3, 5},
	{}
};

const std::vector<int> pc5_o_m_sol[7] = {
	{3},
	{4},
	{6},
	{6},
	{0},
	{1, 2, 5},
	{}
};

const std::vector<int> pc5_o_next = {
	46, // pc6_block,
	48 // pc6_snake
};

const std::vector<int> pc5_o_m_next = {
	47, // pc6_block_m,
	49 // pc6_snake_m
};

void pc5_o(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 5, &out, pc5_o_sol)) {
		for (int i = 0; i < pc5_o_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc5_o_next[i]));
		}
	}
}

void pc5_o_m(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 5, &out, pc5_o_m_sol)) {
		for (int i = 0; i < pc5_o_m_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc5_o_m_next[i]));
		}
	}
}
