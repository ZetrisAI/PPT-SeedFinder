#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <tuple>
#include <vector>
#include "../typedef.h"

#include "traverse.h"
#include "bag1/bag1.h"
#include "bag2/bag2.h"
#include "bag3/bag3.h"
#include "bag4/bag4.h"
#include "bag5/bag5.h"
#include "bag6/bag6.h"
#include "bag7/bag7.h"
#include "bag8/bag8.h"
#include "bag9/bag9.h"

#define SOLUTION_COUNT 51
#define SOLUTION_SUCCESS (SOLUTION_COUNT - 1)
extern void (*const solution_solvers[SOLUTION_COUNT])(int*, int, solution*);

extern solution solution_cache[SOLUTION_COUNT][BAG_MAX][9];
extern solution solution_start;

#endif
