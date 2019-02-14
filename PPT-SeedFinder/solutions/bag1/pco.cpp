#include "pco.h"

const std::vector<int> pco_sol[7] = {
	{4},
	{2},
	{},
	{},
	{1},
	{3},
	{0, 5, 6}
};

const std::vector<int> pco_m_sol[7] = {
	{3},
	{4},
	{},
	{},
	{0},
	{2},
	{1, 5, 6}
};

const std::vector<int> pco_next = {
	12, // pc1_i_j_slope_l,
	13  // pc1_i_j_flat_l
};

const std::vector<int> pco_m_next = {
	14, // pc1_i_l_slope_j,
	15  // pc1_i_l_flat_j
};

void pco(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 6, &out, pco_sol)) {
		for (int i = 0; i < pco_next.size(); i++) {
			next->push_back(std::make_tuple(out, pco_next[i]));
		}
	}
}

void pco_m(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 6, &out, pco_m_sol)) {
		for (int i = 0; i < pco_m_next.size(); i++) {
			next->push_back(std::make_tuple(out, pco_m_next[i]));
		}
	}
}
