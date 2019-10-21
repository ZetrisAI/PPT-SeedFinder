#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <thread>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long ulong;

typedef struct {
	unsigned char moves : 7;
	bool solution_exists : 1;
	unsigned char holds : 4;
	unsigned char rotates : 4;
} solution;

typedef struct {
	uint moves = 0;
	uint rotates = 0;
	uint holds = 0;
} pc_solution;

typedef struct {
	uint rng;
	uint moves;
	uint rotates;
	uint holds;
} rng_solution;

bool operator <(const rng_solution& x, const rng_solution& y) {
	uint xsum = x.moves + x.rotates + x.holds;
	uint ysum = y.moves + y.rotates + y.holds;

	if (xsum == ysum) {
		if (x.moves == y.moves) {
			if (x.holds == y.holds) {
				return x.rotates < y.rotates;
			}
			return x.holds < y.holds;
		}
		return x.moves < y.moves;
	}

	return xsum < ysum;
}

#define BIN_MAX 0x0BBBE1C0u
#define BIN_ELEMENT 8 * sizeof(solution)

const uint BIN_OFFSETS[15] = {
	0x00000000,
	0x00409980,
	0x00813300,
	0x00F23FA0,
	0x01B40C20,
	0x023BB4E0,
	0x02FD8160,

	0x036E8E00,
	0x0532C080,
	0x05A3CD20,
	0x0614D9C0,
	0x07D90C40,
	0x08775120,
	0x0B1D9CE0,

	BIN_MAX
};

const int FACT[8] = {1, 1, 2, 6, 24, 120, 720, 5040};

#define THREADS 3

#define RNG_MAX 0xFFFFFFFFu

#endif