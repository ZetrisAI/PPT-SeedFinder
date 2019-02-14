#include "solutions.h"

void solution_success(int*, int, solution*) {};

void (*const solution_solvers[SOLUTION_COUNT])(int*, int, solution*) = {
	grace,
	grace_i,
	pco,
	pco_m,
	flat_loj,
	flat_loj_m,
	flat_jol,
	flat_jol_m,

	pc1_j_slope_l,
	pc1_j_flat_l,
	pc1_l_slope_j,
	pc1_l_flat_j,
	pc1_i_j_slope_l,
	pc1_i_j_flat_l,
	pc1_i_l_slope_j,
	pc1_i_l_flat_j,
	pc1_z_slope_l,
	pc1_z_flat_l,
	pc1_s_slope_j,
	pc1_s_flat_j,

	pc2_sl_s,
	pc2_sl_z,
	pc2_fl_1,
	pc2_fl_2,
	pc2_sj_s,
	pc2_sj_z,
	pc2_fj_1,
	pc2_fj_2,

	double_s,
	double_z,

	pc3_s,
	pc3_s_i,
	pc3_z,
	pc3_z_i,

	pc4_l,
	pc4_l_i,
	pc4_j,
	pc4_j_i,

	regrace,
	regrace_i,

	pc5,
	pc5_m,
	pc5_o,
	pc5_o_m,

	pc6,
	pc6_m,
	pc6_block,
	pc6_block_m,
	pc6_snake,
	pc6_snake_m,

	solution_success
};

solution solution_cache[SOLUTION_COUNT][BAG_MAX][9];

solution solution_start = {
	std::make_tuple(-1, 0),
	std::make_tuple(-1, 1),
	std::make_tuple(-1, 2),
	std::make_tuple(-1, 3),
	std::make_tuple(-1, 4),
	std::make_tuple(-1, 5),
	std::make_tuple(-1, 6),
	std::make_tuple(-1, 7)
};
