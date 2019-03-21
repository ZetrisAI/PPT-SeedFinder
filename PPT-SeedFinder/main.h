#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <mutex>
#include <thread>
#include <vector>

typedef unsigned int uint;

#define BIN_MAX 0x0BBBE1C0

const int BIN_OFFSETS[15] = {
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