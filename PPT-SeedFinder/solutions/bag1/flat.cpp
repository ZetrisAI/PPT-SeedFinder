#include "flat.h"

const std::vector<int> flat_loj_sol[7] = {
	{6},
	{0, 1, 2, 4},
	{5},
	{},
	{6},
	{3},
	{}
};

const std::vector<int> flat_loj_m_sol[7] = {
	{0, 1, 2, 4},
	{6},
	{5},
	{},
	{6},
	{3},
	{}
};

const std::vector<int> flat_jol_sol[7] = {
	{6},
	{0, 1, 3, 4},
	{},
	{5},
	{6},
	{2},
	{}
};

const std::vector<int> flat_jol_m_sol[7] = {
	{0, 1, 3, 4},
	{6},
	{},
	{5},
	{6},
	{2},
	{}
};

const std::vector<int> flat_next = {
	16, // pc1_z_slope_l
	17  // pc1_z_flat_l
};

const std::vector<int> flat_m_next = {
	18, // pc1_s_slope_j
	19  // pc1_s_flat_j
};

void flat_loj(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 1, &out, flat_loj_sol)) {
		for (int i = 0; i < flat_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_next[i]));
		}
	}
}

void flat_loj_m(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 0, &out, flat_loj_m_sol)) {
		for (int i = 0; i < flat_m_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_m_next[i]));
		}
	}
}


void flat_jol(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 1, &out, flat_jol_sol)) {
		for (int i = 0; i < flat_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_next[i]));
		}
	}
}

void flat_jol_m(int* bag, int hold, solution* next) {
	int out;
	if (traverse_with_preserve(bag, hold, 0, &out, flat_jol_m_sol)) {
		for (int i = 0; i < flat_m_next.size(); i++) {
			next->push_back(std::make_tuple(out, flat_m_next[i]));
		}
	}
}
