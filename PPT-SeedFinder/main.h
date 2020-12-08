#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <thread>
#include <vector>
#include <boost/thread.hpp>

//#define SEARCH_SPECIFIC_SEED 0u
#define PPT2 1
#define REASONABLE_DT_ASSUMPTIONS 0

#define ALLOW_INVALID_SEEDS 0
#define USE_MEMORY_MAP 0

#if USE_MEMORY_MAP
	#include <boost/iostreams/device/mapped_file.hpp>
#endif

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ulong;

#define SET_ITERATIONS 10

typedef struct {
	uchar frames : 7;
	bool solution_exists : 1;
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
	int tetrises;
	int frames;

	int ending[SET_ITERATIONS];
	int holds[SET_ITERATIONS];
	int costs[SET_ITERATIONS];
} rng_solution;

bool operator <(const rng_solution& x, const rng_solution& y) {
	return (x.tetrises == y.tetrises)
		? (x.frames == y.frames)
			? x.rng < y.rng
			: x.frames < y.frames
		: x.tetrises > y.tetrises;
}

const int FACT[8] = {1, 1, 2, 6, 24, 120, 720, 5040};

#define THREADS 12

#define MOV_FILENAME "D:\\all_mov.bin"

#ifdef SEARCH_SPECIFIC_SEED
	#define RNG_MAX SEARCH_SPECIFIC_SEED

#elif ALLOW_INVALID_SEEDS
	#define RNG_MAX 0xFFFFFFFFu

#elif PPT2
	#define RNG_MAX 0x7FFFFFFFu

#else // PPT1
	#define RNG_MAX 0xFFFFu
#endif

#endif