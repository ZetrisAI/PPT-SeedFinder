#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <thread>
#include <vector>

#define USE_MEMORY_MAP 1

#if USE_MEMORY_MAP
	#include <boost/iostreams/device/mapped_file.hpp>
#endif

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ulong;

typedef struct {
	bool solution_exists : 8;
	char piece_infos[10];
} solution;

#define BIN_ELEMENT 8llu * sizeof(solution)

const ulong BIN_OFFSETS[15] = {
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

	#define BIN_MAX 0x0BBBE1C0llu
	BIN_MAX
};

#define BIN_SIZE BIN_MAX * BIN_ELEMENT

typedef struct {
	uint rng;
	uint frames;
} rng_solution;

bool operator <(const rng_solution& x, const rng_solution& y) {
	return x.frames < y.frames;
}

const int FACT[8] = {1, 1, 2, 6, 24, 120, 720, 5040};

#define THREADS 10

#define MOV_FILENAME "D:\\all_mov.bin"
#define RNG_MAX 0xFFFFu

#endif