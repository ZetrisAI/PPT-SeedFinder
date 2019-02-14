#include "pc6_snake.h"

const std::vector<int> pc6_snake_sol[8] = {
	{},
	{},
	{1},
	{6},
	{0},
	{6},
	{2, 4},
	{6}
};

const std::vector<int> pc6_snake_m_sol[8] = {
	{},
	{},
	{6},
	{0},
	{1},
	{6},
	{3, 4},
	{6}
};

void pc6_snake(int* bag, int hold, solution* next) {
	if (hold != 5) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc6_snake_sol)) {
		for (int i = 0; i < pc6_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc6_next[i]));
		}
	}
}

void pc6_snake_m(int* bag, int hold, solution* next) {
	if (hold != 5) return;

	int out;
	if (traverse_preserved(bag, hold, &out, pc6_snake_m_sol)) {
		for (int i = 0; i < pc6_next.size(); i++) {
			next->push_back(std::make_tuple(out, pc6_next[i]));
		}
	}
}
