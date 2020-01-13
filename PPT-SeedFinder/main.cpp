#include "main.h"

solution* bin;

std::vector<rng_solution> solutions;
boost::mutex solutions_locker;

int max_tetrises = SET_ITERATIONS;

const char pieceSymbols[7] = {'S', 'Z', 'J', 'L', 'T', 'O', 'I'};

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

	for (int x = 0; x < SET_ITERATIONS * 10 + 1;) {
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

		for (int i = 0; i < 7 && x < SET_ITERATIONS * 10 + 1; i++)
			set[x++] = bag[i];
	}
}

uint frames_calculate(char* pieces) { 
	uint cost = 0;

	for (int i = 0; i < 10; i++) {
		uint r = pieces[i] >> 1 & 7;
		uint m = pieces[i] >> 4 & 15;

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

void set_iterate(int* set, int index, int hold, rng_solution candidate) {
	int pc = index % 7;
	int s = index * 10;

	if (hold != -1) {
		pc += 7;
		s += 1;
	}

	ulong offset = (ulong)bin + (BIN_OFFSETS[pc] + (ulong)set_encode(&set[s], index, hold)) * BIN_ELEMENT;

	for (int i = hold != -1; i < 8; i++) {
		solution result = *((solution*)(offset + i * sizeof(solution)));
		rng_solution next_candidate = candidate;

		if (result.solution_exists) {
			next_candidate.tetrises++;
			next_candidate.frames += next_candidate.costs[index] = frames_calculate(result.piece_infos);
			next_candidate.tets[index] = true;
		}
		next_candidate.holds[index] = i - 1;

		if (index == SET_ITERATIONS - 1) {
			if (next_candidate.tetrises >= max_tetrises) {
				solutions_locker.lock();
				solutions.push_back(next_candidate);
				solutions_locker.unlock();
			}

		} else if (index - next_candidate.tetrises < SET_ITERATIONS - max_tetrises)
			set_iterate(set, index + 1, i - 1, next_candidate);
	}
}

void rng_search(uint i) {
	printf("  > Thread %d started\n", std::this_thread::get_id());

	do {
		rng_progress = i;
		
		int set[SET_ITERATIONS * 10 + 1];
		rng_generate(i, set);

		set_iterate(set, 0, -1, {i});

	} while ((i += THREADS) - THREADS < RNG_MAX - THREADS + 1);
}

void rng_check_progress() {
	while (true) {
		(void)getchar();
		printf("\n   > Current RNG: 0x%08X\n   > Found RNGs: %llu\n\n", rng_progress, solutions.size());
	}
}

void print_solution(rng_solution* solution) {
	int set[SET_ITERATIONS * 10 + 1];
	rng_generate(solution->rng, set);

	printf("  > 0x%08X (tet: %u; cost: %u; path:", solution->rng, solution->tetrises, solution->frames);

	for (int j = 0; j < SET_ITERATIONS; j++)
		printf(" %d%c(%d)", solution->tets[j], pieceSymbols[solution->holds[j]], solution->costs[j]);

	printf(") - ");

	for (int j = 0; j < SET_ITERATIONS * 10 + 1; j++)
		printf("%c", pieceSymbols[set[j]]);

	printf("\n");
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

	printf("  > Done\n\n > Starting RNG search with Tetris count %u...\n", max_tetrises);

	std::thread progress(rng_check_progress);

	do {
		std::thread threads[THREADS];
		for (int i = 0; i < THREADS; i++) {
			threads[i] = std::thread(rng_search, i);
		}

		for (int i = 0; i < THREADS; i++) {
			if (threads[i].joinable()) threads[i].join();
		}

		if (solutions.empty()) printf("\n > Nothing found, restarting RNG search with Tetris count %u\n", --max_tetrises);
	} while (solutions.empty());

	printf("  > Done\n\n > Found a total of %llu RNG values\n  > Sorting results...\n", solutions.size());

	std::sort(solutions.begin(), solutions.end());

	std::vector<rng_solution> optimal_solutions;

	for (int i = 0; i < solutions.size(); i++) {
		bool add = true;

		for (int j = 0; j < optimal_solutions.size(); j++) {
			if (optimal_solutions[j].rng == solutions[i].rng) {
				add = !(optimal_solutions[j] < solutions[i]);
				if (!add) break;
			}
		}

		if (add) optimal_solutions.push_back(solutions[i]);
	}

	printf("\n > Found %llu optimal solutions\n", optimal_solutions.size());

	for (int i = 0; i < optimal_solutions.size(); i++) {
		print_solution(&optimal_solutions[i]);
	}

	#if USE_MEMORY_MAP
		file.close();
	#endif

	exit(0);
}
