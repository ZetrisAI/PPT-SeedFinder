#include "main.h"

solution* bin;

std::vector<rng_solution> solutions;

int perm_encode(int n, int* perm) {
	int encoded = 0;
	char seen[7] = {};

	for (int i = 0; i < n; i++) {
		int x = perm[i];
		for (int j = 0; j <= perm[i]; ++j) x -= seen[j];
		encoded += x * (FACT[6 - i] / FACT[7 - n]);

		seen[perm[i]] = 1;
	}

	return encoded;
}

int set_encode(int* set, int index, int hold) {
	int size[4] = {}, encoded[4];

	int i = 0;
	int c_max = 11;

	if (hold != -1) {
		size[0] = 1;
		encoded[0] = hold;
		i = 1;
		c_max = 10;
	}

	int s = index * 10 + i;
	int c = 0;

	for (; i < 4; i++) {
		int x = 7 - (s % 7);
		if (x > c_max - c) x = c_max - c;

		size[i] = x;
		encoded[i] = perm_encode(x, &set[c]);

		s += x;
		c += x;

		if (c == c_max) break;
	}

	int result = 0;
	int multiplier = 1;

	if (hold != -1) index += 7;

	for (int i = 3; i >= 0; i--) {
		if (size[i] != 0) {
			result += multiplier * encoded[i];
			multiplier *= FACT[7] / FACT[7 - size[i]];
		}
	}

	return result;
}

uint rng_progress;

uint rng_next(uint rng) {
	return rng * 0x5D588B65 + 0x269EC3;
}

void rng_generate(uint rng, int* set) {
	for (int i = 0; i < 1973; i++) // Global RNG to Piece generation RNG
		rng = rng_next(rng);

	for (int x = 0; x < 105;) {
		int bag[7];
		for (int i = 0; i < 7; i++) bag[i] = i;

		for (int i = 0; i < 7; i++) {
			rng = rng_next(rng);

			int newIndex = ((((rng) >> 16) * (7 - i)) >> 16) + i;

			int newValue = bag[newIndex];
			int oldValue = bag[i];
			bag[i] = newValue;
			bag[newIndex] = oldValue;
		}

		for (int i = 0; i < 7; i++) set[x++] = bag[i];
	}
}

uint frames_calculate(char* pieces) { 
	uint cost = 0;

	for (int i = 0; i < 10; i++) {
		uint r = pieces[i] >> 1 && 8;
		uint m = pieces[i] >> 4 && 16;

		cost += pieces[i] & 1;

		// note: we count the frame we hard drop on
		if (m == 0 && r == 0) {
			cost += 1;

		} else if (m >= r) {
			cost += m * 2;

		} else {
			cost += r * 2 - 1;
		}
	}

	return cost;
}

void set_iterate(int* set, int index, int hold, uint rng, uint cost) {
	int pc = index % 7;
	int s = index * 10;

	if (hold != -1) {
		pc += 7;
		s += 1;
	}

	ulong offset = (ulong)bin + (BIN_OFFSETS[pc] + (ulong)set_encode(&set[s], index, hold)) * BIN_ELEMENT;

	for (int i = 0; i < 8; i++) {
		solution result = *((solution*)(offset + i * sizeof(solution)));

		if (result.solution_exists) {
			uint new_cost = cost + frames_calculate(result.piece_infos);

			if (index == 9) {
				printf("  > Found 0x%04x! %d\n", rng, new_cost);

				rng_solution solved;
				solved.rng = rng;
				solved.frames = new_cost;
				solutions.push_back(solved);

			} else {
				set_iterate(set, index + 1, i - 1, rng, new_cost);
			}
		}
	}
}

void rng_search(uint i) {
	printf("  > Thread %d started\n", std::this_thread::get_id());

	do {
		rng_progress = i;
		
		int set[105];
		rng_generate(i, set);

		uint cost = 0;

		set_iterate(set, 0, -1, i, cost);

	} while ((i += THREADS) - THREADS < RNG_MAX - THREADS + 1);
}

void rng_check_progress() {
	while (true) {
		(void)getchar();
		printf("\n   > Current RNG: 0x%04x\n   > Found RNGs: %llu\n\n", rng_progress, solutions.size());
	}
}

int main() {
	printf("\n Puyo Puyo Tetris Sprint - 10 Tetris PC RNG Seed Finder\n by mat1jaczyyy with help from knewjade and ChiCubed\n\n > Loading binary PC knowledge base...\n");

	#if USE_MEMORY_MAP
		boost::iostreams::mapped_file_source file;

		file.open(MOV_FILENAME, BIN_SIZE);

		if (!file.is_open()) {
			printf("  > Failed to open PC knowledge base!\n");
			exit(1);
		}

		bin = (solution*)file.data();

	#else
		printf("  > Allocating %llu bytes (%.2fGB) of memory...\n", BIN_SIZE, (double)BIN_SIZE / (1024. * 1024 * 1024));

		bin = (solution*)malloc(BIN_SIZE);
		if (bin == NULL) {
			printf("  > Failed to allocate memory!\n");
			exit(1);
		}

		FILE* handle = fopen(MOV_FILENAME, "rb");
		if (handle == NULL) {
			printf("  > Failed to open PC knowledge base!\n");
			exit(1);
		}

		(void)fread(bin, BIN_MAX * BIN_ELEMENT, 1, handle);
		fclose(handle);
	#endif

	printf("  > Done\n\n > Starting RNG search...\n");

	std::thread threads[THREADS];
	for (int i = 0; i < THREADS; i++) {
		threads[i] = std::thread(rng_search, i);
	}
	std::thread progress(rng_check_progress);

	for (int i = 0; i < THREADS; i++) {
		if (threads[i].joinable()) threads[i].join();
	}

	printf("  > Done\n\n > Found a total of %llu RNG values\n\n > Sorting results...\n", solutions.size());

	std::sort(solutions.begin(), solutions.end());

	for (int i = 0; i < solutions.size(); i++) {
		int set[105];
		rng_generate(solutions[i].rng, set);
		
		printf("  > 0x%04x (cost: %u) - ", solutions[i].rng, solutions[i].frames);
		for (int j = 0; j < 101; j++) {
			printf("%d ", set[j]);
		}
		printf("\n");
	}

	#if USE_MEMORY_MAP
		file.close();
	#endif

	exit(0);
}
