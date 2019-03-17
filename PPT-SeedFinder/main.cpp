#include "main.h"

uint rng_progress;

void bag_decode(int index, int* bag) {
	for (int i = 7; i--;) {
		bag[i] = (index % FACT[7 - i]) / FACT[6 - i];
		for (int j = i + 1; j < 7; ++j) {
			if (bag[j] >= bag[i]) bag[j]++;
		}
	}
}

int bag_encode(int* bag) {
	int encoded = 0;
	char seen[7] = {};

	for (int i = 0; i < 7; ++i) {
		int x = bag[i];
		for (int j = 0; j <= bag[i]; ++j) x -= seen[j];
		encoded += x * FACT[6 - i];

		seen[bag[i]] = 1;
	}

	return encoded;
}

uint rng_next(uint rng) {
	return rng * 0x5D588B65 + 0x269EC3;
}

const char piecemap[7] = {
	'S', 'Z', 'J', 'L', 'T', 'O', 'I'
};

std::mutex sol_found_lock;

void rng_iterate(uint rng, solution solutions, bool* ret, std::vector<int> vec, uint init, int prev) {
	int bag[7];
	for (int i = 0; i < 7; i++) {
		bag[i] = i;
	}

	for (int i = 0; i < 7; i++) {
		rng = rng_next(rng);

		int newIndex = ((((rng) >> 16) * (7 - i)) >> 16) + i;

		int newValue = bag[newIndex];
		int oldValue = bag[i];
		bag[i] = newValue;
		bag[newIndex] = oldValue;
	}

	int bag_i = bag_encode(bag);

	for (int i = 0; i < solutions.size(); i++) {
		int sol = std::get<1>(solutions[i]);
		int hold = std::get<0>(solutions[i]) + 1;

		if (sol == SOLUTION_SUCCESS) {
			std::lock_guard<std::mutex> lock(sol_found_lock);

			printf("/* --------------------------------------------------- */\nRNG %u (0x%08x):\nSolution path:", init, init);
			for (int j = 0; j < vec.size(); j++) {
				printf(" %d", vec[j]);
			}

			int previous[7];
			bag_decode(prev, previous);

			if (previous[0] != 6) {
				int temp = previous[0];
				previous[0] = previous[1];
				previous[1] = temp;
			}

			printf("\n16 Lines left, board is empty.\nPieces left:\n %c%c%c%c%c%c\n", piecemap[previous[1]], piecemap[previous[2]], piecemap[previous[3]], piecemap[previous[4]], piecemap[previous[5]], piecemap[previous[6]]);
			
			for (int j = 0; j < 7; j++) {
				for (int k = 0; k < 7; k++) {
					printf("%c", piecemap[bag[k]]);
				}
				printf("\n");

				for (int k = 0; k < 7; k++) {
					bag[k] = k;
				}

				for (int k = 0; k < 7; k++) {
					rng = rng_next(rng);

					int newIndex = ((((rng) >> 16) * (7 - k)) >> 16) + k;

					int newValue = bag[newIndex];
					int oldValue = bag[k];
					bag[k] = newValue;
					bag[newIndex] = oldValue;
				}
			}
			printf("\n");

			*ret = true;
		}

		if (solution_cache[sol][bag_i][hold].size()) {
			std::vector<int> copy = vec;
			copy.push_back(sol);
			rng_iterate(rng, solution_cache[sol][bag_i][hold], ret, copy, init, bag_i);
		}
	}
}

uint sol = 0;

void rng_search(uint i) {
	printf("  > Thread %d started\n", std::this_thread::get_id());
	do {
		bool ret = false;
		rng_progress = i;
		std::vector<int> empty;
		rng_iterate(i, solution_start, &ret, empty, i, 0);

		if (ret) {
			sol++;
		}
	} while ((i += THREADS) - THREADS < RNG_MAX - THREADS + 1);
}

void rng_check_progress() {
	while (true) {
		getchar();
		printf("   > Current RNG: %u\n   > Found RNG: %u\n\n", rng_progress, sol);
	}
}

int main() {
	printf("\n Puyo Puyo Tetris Sprint 7PC RNG Seed Finder\n by mat1jaczyyy with help from Galactoid and ChiCubed\n\n");

	printf(" > Precaching bag solution outcomes...\n");

	int bag[7];
	for (int i = 0; i < SOLUTION_COUNT; i++) {
		int sol = 0;
		for (int j = 0; j < 5040; j++) {
			bag_decode(j, bag);
			for (int k = HOLD_EMPTY; k <= HOLD_FIRST_BAG; k++) {
				int copy[7];
				std::copy(std::begin(bag), std::end(bag), std::begin(copy));

				(*solution_solvers[i])(copy, k, &solution_cache[i][j][k + 1]);
			}
		}
	}

	printf("  > Done\n\n > Starting RNG search...\n");

	std::thread threads[THREADS];
	for (int i = 0; i < THREADS; i++) {
		threads[i] = std::thread(rng_search, i);
	}
	std::thread progress(rng_check_progress);

	for (int i = 0; i < THREADS; i++) {
		if (threads[i].joinable()) threads[i].join();
	}

	printf("  > Done\n\n > Found a total of %u RNG values\n\n", sol);

	exit(0);
}
